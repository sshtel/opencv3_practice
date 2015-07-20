#ifndef __FACE_DETECTOR_CL_H
#define __FACE_DETECTOR_CL_H

#include <opencv2/opencv.hpp>
#include <FaceDetector.hpp>

class FaceDetectorCL : public FaceDetector{

private:
	
	cv::Mat matSrc_;
	
	cv::UMat umatSrc_;
	cv::UMat umatGray_;
	
	bool isGray(cv::Mat &src);
	
public:
	explicit FaceDetectorCL(std::string name);
	

	int load(std::string path);
	void setSrcImg(cv::Mat &src, double scale);
	int cutFace();
	int cutEyes();
	
	cv::Mat& resultMat() { return this->matSrc_; }

};


#endif