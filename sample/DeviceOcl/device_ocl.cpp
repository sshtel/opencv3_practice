#include <opencv2/core/ocl.hpp>
#include "device_ocl.h"

bool DeviceOcl::setDevice(const char* platformName, const int deviceType){
	std::vector<cv::ocl::PlatformInfo> platformsInfo;
	cv::ocl::getPlatfomsInfo(platformsInfo);
	
	

	//find platform and device using PlatformInfo::platformVersion
	std::vector<cv::ocl::PlatformInfo>::iterator itr = platformsInfo.begin();
	for(; itr != platformsInfo.end(); itr++){
		const cv::ocl::PlatformInfo pInfo = *itr;
		cv::String name = pInfo.name();
		int pos = name.find(platformName, 0);
		if(pos >= 0){
			//platform found!
			
			for(int dev_cnt = 0; dev_cnt < pInfo.deviceNumber(); dev_cnt++){
				cv::ocl::Device device;
				
				pInfo.getDevice(device, dev_cnt);
				if (device.type() == deviceType){
					device.set(device.ptr());
					
					cv::ocl::setUseOpenCL(true);
					return cv::ocl::useOpenCL();
				}
			}
		}
	}
	
	cv::ocl::setUseOpenCL(false);
	return false;
}

bool DeviceOcl::setDevice(const int vendor, const int deviceType){
	std::vector<cv::ocl::PlatformInfo> platformsInfo;
	cv::ocl::getPlatfomsInfo(platformsInfo);
	
	//find platform and device using PlatformInfo::platformVersion
	std::vector<cv::ocl::PlatformInfo>::iterator itr = platformsInfo.begin();
	for (; itr != platformsInfo.end(); itr++){
		const cv::ocl::PlatformInfo pInfo = *itr;
		//platform found!

		for (int dev_cnt = 0; dev_cnt < pInfo.deviceNumber(); dev_cnt++){
			cv::ocl::Device device;
			pInfo.getDevice(device, dev_cnt);
			if (device.type() == deviceType && device.vendorID() == vendor){
				device.set(device.ptr());
				cv::ocl::setUseOpenCL(true);
				return cv::ocl::useOpenCL();
			}
		}
		
	}
	cv::ocl::setUseOpenCL(false);
	return false;
}
