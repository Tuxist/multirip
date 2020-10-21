#include <cstring>
#include <climits>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <iostream>

#include "cddevice_linux.h"

using namespace MultiRip;

CdDeviceLinux::CdDeviceLinux(const char *device) : CdDevice(){
    CDMSF={0};
    Data = new char[CD_FRAMESIZE_RAW];
    Device = open(device,O_RDONLY | O_NONBLOCK | O_EXCL);
    int i;
    for (i = 0; (i < 5) && (Device == -1 && errno == EBUSY); i++) {
        Device = open(device, O_RDONLY | O_NONBLOCK | O_EXCL);
        sleep(1);
    }

    if(i<5 && Device<0) {
        throw std::string("Please umount Device:").append(device);
    }  
}

CdDeviceLinux::~CdDeviceLinux(){
    ioctl(Device,CDROM_CLEAR_OPTIONS,0);
    close(Device);
    delete[] Data;
}

bool CdDeviceLinux::checkDevice(){
    ioctl(Device, CDROMSTART, 0);
    switch(ioctl(Device, CDROM_DRIVE_STATUS, CDSL_NONE)){
        case CDS_DISC_OK:
            return true;
        default:
            return false;
    };
}

void CdDeviceLinux::Read(char **buffer){
    union {
       struct cdrom_msf* cdmsf;
       char              buffer[CD_FRAMESIZE_RAW];
    } cdread{0};
    cdread.cdmsf=&CDMSF;
    if(ioctl(Device,CDROMREADRAW,&cdread)<0)
        throw std::string("Cannot read: ").append(strerror(errno));
    memcpy(Data,&cdread.buffer,CD_FRAMESIZE_RAW);
    *buffer=Data;
}

void CdDeviceLinux::nextSector(){
    if(CDMSF.cdmsf_frame0<CD_FRAMES){
        if(CDMSF.cdmsf_sec0<CD_SECS){
            CDMSF.cdmsf_sec0++;
        }else{
            CDMSF.cdmsf_min0++;
            CDMSF.cdmsf_sec0=0;
        }
        CDMSF.cdmsf_frame0=0;
    }else{
        CDMSF.cdmsf_frame0++;
    }
}

void CdDeviceLinux::eject(){
    int state = ioctl(Device, CDROMSTOP, 0);
    do {
        state=ioctl(Device, CDROMEJECT,0);
        if(state==ENOSYS)
            throw std::string("Cannot eject: ").append(strerror(errno));
    }while(state==EBUSY);
}

void CdDeviceLinux::setSpeed(int speed){
    ioctl(Device,CDROM_SELECT_SPEED,speed);
}

int CdDeviceLinux::getBlockSize(){
    return CD_FRAMESIZE_RAW;
}

void CdDeviceManager::scanDevices(CdDevice **firstdevice,CdDevice **lastdevice){
    DIR * d = opendir("/dev"); // open the path
    if(d==NULL) 
        throw std::string("No Device Path found");
    struct dirent * dir;
    while ((dir = readdir(d)) != NULL){
        if(dir-> d_type == DT_BLK && strncmp("sr",dir->d_name,2)==0){
            char *tmp = new char[384];
            snprintf(tmp,strlen(dir->d_name)+6,"/dev/%s",dir->d_name);
            try{
                CdDeviceLinux *dev=new CdDeviceLinux(tmp);
                if(*firstdevice){
                    (*lastdevice)->nextCdDevice=dev;
                    (*lastdevice)=(*lastdevice)->nextCdDevice;
                }else{
                    (*firstdevice)=dev;
                    (*lastdevice)=*firstdevice;
                }
            }catch(std::string e){
                std::cerr << "Cannot create Device: " << e << std::endl;
            }
            delete[] tmp;
        }
    }
    closedir(d);
}
