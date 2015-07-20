#ifndef __CASCADE_CLASSIFIER_H
#define __CASCADE_CLASSIFIER_H

#include <opencv2/opencv.hpp>

class FaceDetector{

protected:
	std::string name_;
	std::string videoFile_;
	std::string dataPath_;
	cv::CascadeClassifier face_classifier_;
	std::vector<cv::Rect> faces_;
public:
	std::string name() { return name_; }
	std::string videoFile() { return videoFile_; }
	void setVideoFile(std::string file) { videoFile_ = file; }


	virtual int load(std::string path) = 0;
	virtual void setSrcImg(cv::Mat &src, double scale) = 0;
	int doWork();
	virtual int cutFace() = 0;
	virtual int cutEyes() = 0;

	virtual cv::Mat& resultMat() = 0;
	void getResultFaces(std::vector<cv::Rect> &faces);
	std::vector<cv::Rect>& getResultFaces(){ return faces_; }
};

#endif
