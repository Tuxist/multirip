#include <cstddef>
#include <string>

#include "cddevice.h"

#ifdef Linux
#include "os/cddevice_linux.h" 
#endif

using namespace MultiRip;

CdDevice::CdDevice(){
    nextCdDevice=NULL;
}

CdDevice::~CdDevice(){
    delete nextCdDevice;
}

CdDevice *CdDevice::getnextCdDevice(){
  return nextCdDevice;
}

CdDeviceManager::CdDeviceManager(){
    firstCdDevice=NULL;
    lastCdDevice=firstCdDevice;
    scanDevices(&firstCdDevice,&lastCdDevice);
    if(!firstCdDevice)
        throw std::string("No Devices detected");
}

CdDeviceManager::~CdDeviceManager(){
   delete firstCdDevice;
   lastCdDevice=NULL;
}

CdDevice *CdDeviceManager::getCdDevice(){
  return  firstCdDevice;
}
