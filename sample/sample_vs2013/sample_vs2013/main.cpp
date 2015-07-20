#include <iostream>
#include <opencv2/opencv.hpp>
#include <device_ocl.h>

using namespace cv;
using namespace std;


#define IMAGE_ROOT "..\\..\\..\\images"
#define HOG_IMG IMAGE_ROOT"\\hog_1.jpg"
#define HOG_VID IMAGE_ROOT"\\pedestrian.mp4"

int main(){

	/*
	set environment variables before run this program.
	OpenCV 3.0 does not provide cv::ocl::setDevice() API
	EX)
	OPENCV_OPENCL_DEVICE=:CPU:0
	OPENCV_OPENCL_DEVICE=:GPU:0
	OPENCV_OPENCL_DEVICE=:GPU:1

	*/

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
	if (devOcl.checkDefaultDevice(vendor, deviceType)) {
		std::cout << " check device OK" << std::endl;
	}


	return 0;
}