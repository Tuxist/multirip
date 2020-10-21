#include <iostream>
#include <fstream> 
#include "cddevice.h"

#define LEADIN 0xF0000000

using namespace MultiRip;

int main(int argc, char* argv[]) {
    try{
        CdDeviceManager *devmng = new CdDeviceManager();
        CdDevice *mydevice = devmng->getCdDevice();
        if(!mydevice->checkDevice())
            return -1;
        std::ofstream out;
        out.open("test.dump");
        int tries=0;
        char *buffer;
        for(int i=0; i<100000; ++i){
            try{
                mydevice->Read(&buffer);
                out.write(buffer,mydevice->getBlockSize());
                mydevice->nextSector();
            }catch(std::string e){
                std::cerr << e << std::endl;
                --i;
                tries++;
                if(tries==5){
                   tries=0;
                   mydevice->nextSector();
                }
            }
        };
        out.close();
        mydevice->eject();
        delete devmng;
    }catch(std::string e){
        std::cerr << e << std::endl;
    }
    return 0;
} 
