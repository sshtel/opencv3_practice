#ifndef __DEVICE_OCL_H
#define __DEVICE_OCL_H

#include <opencv2/core/ocl.hpp>

class DeviceOcl {
public:
	bool setDevice(const char* platformName, int deviceType);
	
	bool setDeviceFromAnyPlatform(int deviceType);

};



#endif