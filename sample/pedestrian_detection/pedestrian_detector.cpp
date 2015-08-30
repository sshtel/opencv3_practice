
/* This sample program is copied and edited from OpenCV 3.0.0 example code
www.opencv.org
*/


#include "pedestrian_detector.h"


using namespace cv;

PedestrianDetector::PedestrianDetector() :
win_stride_(8, 8),
gamma_corr_(true),
make_gray_(true),
scale_(1.02), //1.05
resize_scale_(1.0), //1.0
nlevels_(30), //30
hit_threshold_(0.0), //0.0
gr_threshold_(30), //30
win_size_(WIN_WIDTH_DAIMLER, WIN_HEIGHT_DAIMLER),
hog_(win_size_, Size(16, 16), Size(8, 8), Size(8, 8), 9, 1, -1,
HOGDescriptor::L2Hys, 0.2, gamma_corr_, cv::HOGDescriptor::DEFAULT_NLEVELS)
{
	//setSVMDetector(SVM_DETECTOR::DAIMLER_DETECTOR);
	setSVMDetector(SVM_DETECTOR::DEFAULT_DETECTOR);
}

void PedestrianDetector::setSVMDetector(SVM_DETECTOR detector){
	switch (detector){
	case SVM_DETECTOR::DAIMLER_DETECTOR:
		win_size_.width = WIN_WIDTH_DAIMLER;
		win_size_.height = WIN_HEIGHT_DAIMLER;
		hog_.winSize = win_size_;
		hog_.setSVMDetector(HOGDescriptor::getDaimlerPeopleDetector());
		break;
	case SVM_DETECTOR::DEFAULT_DETECTOR:
		win_size_.width = WIN_WIDTH_DEFAULT;
		win_size_.height = WIN_HEIGHT_DEFAULT;
		hog_.winSize = win_size_;
		hog_.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
		break;
	}
}

void PedestrianDetector::detect(UMat frame, std::vector<Rect> &found){
	UMat img_aux, img;
	
	//work begin
	// Change format of the image
	if (make_gray_) cvtColor(frame, img_aux, COLOR_BGR2GRAY);
	else frame.copyTo(img_aux);

	// Resize image
	if (abs(scale_ - 1.0) > 0.001)
	{
		Size sz((int)((double)img_aux.cols / resize_scale_), (int)((double)img_aux.rows / resize_scale_));
		resize(img_aux, img, sz);
	}
	else { img = img_aux; }
	
	hog_.nlevels = nlevels_;
	
	//hog performance
	hog_.detectMultiScale(img, found, hit_threshold_, win_stride_, Size(0, 0), scale_, gr_threshold_);
	//hog_.detectMultiScale(img, found, hit_threshold_, win_stride_, Size(32, 32), scale_, gr_threshold_);  // If padding is not Size(0,0), OpenCL is not used..
}

void PedestrianDetector::detect(Mat frame, std::vector<Rect> &found){
	Mat img_aux, img;

	//work begin
	// Change format of the image
	if (make_gray_) cvtColor(frame, img_aux, COLOR_BGR2GRAY);
	else frame.copyTo(img_aux);

	// Resize image
	if (abs(scale_ - 1.0) > 0.001)
	{
		Size sz((int)((double)img_aux.cols / resize_scale_), (int)((double)img_aux.rows / resize_scale_));
		resize(img_aux, img, sz);
	}
	else { img = img_aux; }

	hog_.nlevels = nlevels_;

	//hog performance
	hog_.detectMultiScale(img, found, hit_threshold_, win_stride_,
		Size(0, 0), scale_, gr_threshold_);

}