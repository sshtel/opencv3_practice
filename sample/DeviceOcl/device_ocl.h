#ifndef __DEVICE_OCL_H
#define __DEVICE_OCL_H

#include <opencv2/core/ocl.hpp>

class DeviceOcl {
public:
	bool setDevice(const char* platformName, const int deviceType);
	
	bool setDevice(const int vendor, const int deviceType);
	bool checkDefaultDevice(const int vendor, const int deviceType);
};



#endif