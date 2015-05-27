#include <opencv2/core/ocl.hpp>
#include "device_ocl.h"

bool DeviceOcl::setDevice(const char* platformName, int deviceType){
	bool clDeviceFound = false;
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
				cv::ocl::Device dev;
				pInfo.getDevice(dev, dev_cnt);
				if(dev.type() == deviceType){
					cv::ocl::setUseOpenCL(true);
					return true;
				}
			}
		}
	}
	return false;
}
	
bool DeviceOcl::setDeviceFromAnyPlatform(int deviceType){

	//for 2.x
	/*
	int size = platformsInfo.size();
	for(int i=0; i<size; i++){
		const cv::ocl::PlatformInfo *pInfo = platformsInfo.at(i);
		cv::ocl::DevicesInfo devicesInfo;
		int ret = cv::ocl::getOpenCLDevices(devicesInfo, deviceType, pInfo); //get ANY device of that type.
		if(ret) { 
			cv::ocl::setDevice(devicesInfo.at(0));
			return true;
		}
	}
	*/
	return false;
}
