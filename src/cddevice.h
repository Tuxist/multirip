#ifndef CDDEVICE_H
#define CDDEVICE_H 

namespace MultiRip {
    class CdDevice {
    public:
        virtual void Read(char **buffer)=0;
        virtual void eject()=0;
        virtual void setSpeed(int speed)=0;
        virtual int getBlockSize()=0;
        virtual void nextSector()=0;
        virtual bool checkDevice()=0;
        CdDevice *getnextCdDevice();
    protected:
        CdDevice();
        virtual ~CdDevice();
    private:
        CdDevice     *nextCdDevice;
        friend class CdDeviceManager;
    };
    
    class CdDeviceManager {
    public:
        CdDeviceManager();
        virtual ~CdDeviceManager();
        CdDevice *getCdDevice();
    protected:
        static void scanDevices(CdDevice **firstdevice,CdDevice **lastdevice);
        CdDevice *firstCdDevice;
        CdDevice *lastCdDevice;
    };
};
#endif
