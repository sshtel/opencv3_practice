#include <device_ocl.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#ifdef WIN32
#include <windows.h>
#include <process.h>
#include <stdlib.h>
#endif

using namespace cv;
using namespace std;


#define IMAGE_ROOT "..\\..\\..\\images"

#define HELICOPTER IMAGE_ROOT"\\helicopter.jpg"
#define LANDSCAPE1 IMAGE_ROOT"\\landscape1.jpg"
#define LANDSCAPE2 IMAGE_ROOT"\\landscape2.jpg"

void seamless_clone(bool isCL){
	
	cv::Mat dst;
	cv::Mat src = cv::imread(HELICOPTER);
	cv::UMat umatSrc = src.getUMat(cv::ACCESS_READ);
	cv::UMat umatDst = cv::imread(LANDSCAPE2).getUMat(cv::ACCESS_READ);
	cv::UMat umatGray;

	cv::Point p;
	p.x = 0;
	p.y = 0;
	
	cv::UMat umatOutput;

	// Define the mask as a closed polygon
	Point poly[1][7];
	poly[0][0] = Point(603, 562);
	poly[0][1] = Point(668, 528);
	poly[0][2] = Point(789, 536);
	poly[0][3] = Point(805, 636);
	poly[0][4] = Point(758, 701);
	poly[0][5] = Point(667, 712);
	poly[0][6] = Point(590, 664);

	const Point* polygons[1] = { poly[0] };
	int num_points[] = { 7 };
	cv::Mat src_mask = Mat::zeros(src.rows, src.cols, src.depth());

	cv::fillPoly(src_mask, polygons, num_points, 1, Scalar(255, 255, 255));
	
	// The location of the center of the src in the dst
	Point center(800, 100);

	seamlessClone(umatSrc, umatDst, src_mask, center, umatOutput, MIXED_CLONE);
	//seamlessClone(umatSrc, umatDst, src_mask, center, umatOutput, NORMAL_CLONE);

	umatOutput.copyTo(dst);
	cv::imshow("test", dst);

	cv::waitKey(0);
}


int main(){
	bool clDeviceFound = false;

	int deviceType = cv::ocl::Device::TYPE_GPU;
	int vendor = cv::ocl::Device::VENDOR_AMD;

	cv::ocl::Device device = ::ocl::Device::getDefault();
	cv::String vendorName = device.vendorName();
	int type = device.type();

	DeviceOcl devOcl;
	clDeviceFound = devOcl.setDevice(vendor, deviceType);
	
	seamless_clone(clDeviceFound);
	
	return 0;
}
