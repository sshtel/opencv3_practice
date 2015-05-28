#include <FaceDetectorCpu.hpp>
#include <FaceData.hpp>

FaceDetectorCpu::FaceDetectorCpu(std::string name){
	name_ = name;
}

int FaceDetectorCpu::load(std::string path){
	dataPath_ = path;
	
	if(	!face_classifier_.load(this->dataPath_)  ){
		return -1;
	}
	return 0;
}

void FaceDetectorCpu::setSrcImg(cv::Mat &src, double scale){
	this->matSrc_ = src;
}

int FaceDetectorCpu::cutFace(){
	
	cv::cvtColor(matSrc_, matGray_, CV_BGR2GRAY);		
	cv::equalizeHist(matGray_, matGray_);
	cv::GaussianBlur(matGray_, matGray_, cv::Size(3, 3), 1, 1);
	
	std::vector<cv::Rect> faces;
	
	face_classifier_.detectMultiScale(matGray_, faces, 1.1, 
										3,
										CV_HAAR_SCALE_IMAGE,
										cv::Size(150, 150));

	for(int i=0; i<faces.size(); i++) {
        cv::Point lb(faces[i].x + faces[i].width,
                        faces[i].y + faces[i].height);
        cv::Point tr(faces[i].x, faces[i].y);
 
		cv::rectangle(this->matSrc_, lb, tr, cv::Scalar(0,255,0), 3, 4, 0);
    }

	if(faces.size() == 0) return -1;
	return 0;
}

int FaceDetectorCpu::cutEyes(){
	
	cv::cvtColor(matSrc_, matGray_, CV_BGR2GRAY);		
	//cv::equalizeHist(matGray_, matGray_);
	//cv::GaussianBlur(matGray_, matGray_, cv::Size(3, 3), 1, 1);
	std::vector<cv::Rect> faces;
	
	face_classifier_.detectMultiScale(matGray_, faces, 1.1, // must be bigger than 1.0
										3,
										CV_HAAR_SCALE_IMAGE,
										cv::Size(30, 30));

	for(int i=0; i<faces.size(); i++) {
        cv::Point lb(faces[i].x + faces[i].width,
                        faces[i].y + faces[i].height);
        cv::Point tr(faces[i].x, faces[i].y);
 
		cv::rectangle(this->matSrc_, lb, tr, cv::Scalar(0,255,0), 3, 4, 0);
    }
	
	if(faces.size() == 0) return -1;
	return 0;
}

bool FaceDetectorCpu::isGray(cv::Mat &src){
	if(src.channels() == 1) return true;
	return false;
}
