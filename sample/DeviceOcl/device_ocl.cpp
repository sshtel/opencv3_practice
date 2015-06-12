#include <opencv2/core/ocl.hpp>
#include <iostream>
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
					std::cout << "Vendor : " << device.vendorName() << std::endl;
					std::cout << "Type : " << device.type() << std::endl;
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
				/*
				cv::ocl::setDevice(); functions is not provided in v3.0.0
				environment variables are used to choose OpenCL devices instead.
				EX)
				OPENCV_OPENCL_DEVICE=:GPU:0
				*/

				device.set(device.ptr());
				std::cout << "Vendor : " << device.vendorName() << std::endl;
				std::cout << "Type : " << device.type() << std::endl;
				cv::ocl::setUseOpenCL(true);
				return cv::ocl::useOpenCL();
			}
		}
		
	}
	cv::ocl::setUseOpenCL(false);
	return false;
}


bool DeviceOcl::checkDefaultDevice(const int vendor, const int deviceType){
	cv::ocl::Device device = cv::ocl::Device::getDefault();
	int default_vendor = device.vendorID();
	int default_type = device.type();
	cv::String typeName;
	switch (default_type){
		case  cv::ocl::Device::TYPE_CPU: typeName = "CPU"; break;
		case  cv::ocl::Device::TYPE_GPU: typeName = "GPU"; break;
		case  cv::ocl::Device::TYPE_ACCELERATOR: typeName = "ACCELERATOR"; break;
		case  cv::ocl::Device::TYPE_IGPU: typeName = "IGPU"; break;
		case  cv::ocl::Device::TYPE_DGPU: typeName = "DGPU"; break;
		case  cv::ocl::Device::TYPE_ALL: typeName = "ALL"; break;
	}

	std::cout << "Vendor : " << device.vendorName().c_str() << std::endl;
	std::cout << "Vendor ID : " << default_vendor << std::endl;
	std::cout << "type  : " << typeName.c_str() << std::endl;
	std::cout << "type ID : " << default_type << std::endl;

	if (vendor == default_vendor && deviceType == default_type) { return true;  }
	return false;
}