#ifndef __PEDESTRIAN_DETECTOR_H
#define __PEDESTRIAN_DETECTOR_H

/* This sample program is copied and edited from OpenCV 3.0.0 example code
www.opencv.org
*/

#include <opencv2/opencv.hpp>

using namespace cv;

class PedestrianDetector{
	
private:
	bool gamma_corr_ = true;
	bool make_gray_ = true;
	double scale_ = 1.05;
	double resize_scale_ = 1.0;
	int nlevels_ = 13;
	double hit_threshold_ = 1.4;
	int gr_threshold_ = 8;

#define WIN_WIDTH_DAIMLER 48
#define WIN_HEIGHT_DAIMLER WIN_WIDTH_DAIMLER * 2
#define WIN_WIDTH_DEFAULT 64
#define WIN_HEIGHT_DEFAULT WIN_WIDTH_DEFAULT * 2
	Size win_size_;
	Size win_stride_;

public:

	typedef enum {
		DAIMLER_DETECTOR,
		DEFAULT_DETECTOR
	} SVM_DETECTOR;


	bool gamma_corr() {}
	bool make_gray() {}
	double scale() {}
	double resize_scale() {}
	int nlevels() {}
	double hit_threshold() {}
	int gr_threshold() {}

	void setGammaCorr(bool gamma_corr){ this->gamma_corr_ = gamma_corr; }
	void setScale(double scale) { this->scale_ = scale; }
	void setResizeScale(double resize_scale) { this->resize_scale_ = resize_scale; }
	void setNlevels(int nlevels) { this->nlevels_ = nlevels; }
	void setHitThreeshold(double hit_threshold){ this->hit_threshold_ = hit_threshold; }
	void setGroupThreshodl(int gr_threshold){ this->gr_threshold_ = gr_threshold; }

	PedestrianDetector();

private:
	// Create HOG descriptors and detectors here
	HOGDescriptor hog_;
	
public:
	
	void setSVMDetector(SVM_DETECTOR detector);

	void detect(UMat frame, std::vector<Rect> &found);
	void detect(Mat frame, std::vector<Rect> &found);

};


#endif