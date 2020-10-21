#ifndef CDDEVICELINUX_H
#define CDDEVICELINUX_H 

#include "../cddevice.h"

extern "C" {
    #include <linux/cdrom.h>
    #include <scsi/sg.h>
}

namespace MultiRip {
    class CdDeviceLinux : public CdDevice {
    public:
        void Read(char **buffer);
        void eject();
        void setSpeed(int speed);
        int  getBlockSize();
        void nextSector();
        bool checkDevice();
    private:
        struct cdrom_msf CDMSF;
        char *Data;
        CdDeviceLinux(const char *device);
        virtual ~CdDeviceLinux();
        int Device;
        friend class CdDeviceManager;
    };
};
#endif
