#include <iostream>
#include <opencv2/opencv.hpp>
#include <device_ocl.h>

int main(){

	cv::String vendorName;
	int deviceType;
	int vendor;
	bool clDeviceFound = false;
	
	/*
	set environment variables before run this program.
	OpenCV 3.0 does not provide cv::ocl::setDevice() API
	EX)
	OPENCV_OPENCL_DEVICE=:CPU:0
	OPENCV_OPENCL_DEVICE=:GPU:0
	OPENCV_OPENCL_DEVICE=:GPU:1

	*/

	std::string platformName = "AMD";
	//std::string platformName = "Intel";
	deviceType = cv::ocl::Device::TYPE_GPU;
	vendor = cv::ocl::Device::VENDOR_AMD;

	DeviceOcl devOcl;
	if (devOcl.checkDefaultDevice(vendor, deviceType)) {
		std::cout << " check device OK" << std::endl;
	}
	return 0;
}