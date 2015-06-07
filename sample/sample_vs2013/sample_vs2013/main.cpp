#include <iostream>
#include <opencv2/opencv.hpp>
#include <device_ocl.h>

int main(){

	cv::String vendorName;
	int deviceType;
	int vendor;
	bool clDeviceFound = false;
	
	cv::ocl::Device device = cv::ocl::Device::getDefault();
	vendorName = device.vendorName();
	deviceType = device.type();

	std::cout << "Vendor : " << vendorName.c_str() << std::endl;
	std::cout << "type  : " << deviceType << std::endl;
	

	std::string platformName = "AMD";
	//std::string platformName = "Intel";
	deviceType = cv::ocl::Device::TYPE_GPU;
	vendor = cv::ocl::Device::VENDOR_AMD;

	DeviceOcl devOcl;
	//clDeviceFound = devOcl.setDevice(platformName.c_str(), deviceType);
	clDeviceFound = devOcl.setDevice(vendor, deviceType);
	
	return 0;
}