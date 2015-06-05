#ifndef __FACE_DETECTOR_CPU_H
#define __FACE_DETECTOR_CPU_H

#include <opencv2/opencv.hpp>
#include <FaceDetector.hpp>

class FaceDetectorCpu : public FaceDetector{

private:
	
	std::string videoFile_;

	cv::Mat matSrc_;
	cv::Mat matGray_;

	bool isGray(cv::Mat &src);
	
public:
	explicit FaceDetectorCpu(std::string name);
	

	int load(std::string path);
	void setSrcImg(cv::Mat &src, double scale);
	int cutFace();
	int cutEyes();

	cv::Mat& resultMat() { return this->matSrc_; }
	
	void showImage(cv::Mat &mat)
	{
		cv::Mat img(mat);
		cv::imshow("show", img);
		cv::waitKey(0);
	}
	
};


#endif