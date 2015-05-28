#include <FaceDetectorCL.hpp>
#include <FaceData.hpp>

FaceDetectorCL::FaceDetectorCL(std::string name){
	this->name_ = name;
}

int FaceDetectorCL::load(std::string path){
	dataPath_ = path;
	if (!face_classifier_.load(path)) {
        std::cout << "ERROR: Could not load classifier cascade: " << path << std::endl;
        return -1;
    }
	return 0;
}

void FaceDetectorCL::setSrcImg(cv::Mat &src, double scale){
	this->matSrc_ = src;
	if (this->umatSrc_.empty()) this->umatSrc_ = src.getUMat(cv::ACCESS_READ);
}

int FaceDetectorCL::cutFace(){
    cv::cvtColor( umatSrc_, umatGray_, CV_BGR2GRAY );
	cv::equalizeHist(umatGray_, umatGray_);
	cv::GaussianBlur(umatGray_, umatGray_, cv::Size(3, 3), 1, 1);

	std::vector<cv::Rect> faces;
	face_classifier_.detectMultiScale(umatGray_, faces, 1.1,
		3,
		CV_HAAR_SCALE_IMAGE,
		cv::Size(150, 150));
	
	for(int i=0; i<faces.size(); i++) {
        cv::Point lb(faces[i].x + faces[i].width,
                        faces[i].y + faces[i].height);
        cv::Point tr(faces[i].x, faces[i].y);
 		cv::rectangle(this->matSrc_, lb, tr, cv::Scalar(0,255,0), 3, 4, 0);
    }
	
	return 0;
}

int FaceDetectorCL::cutEyes(){

	cv::cvtColor(umatSrc_, umatGray_, CV_BGR2GRAY);
	//cv::equalizeHist( umatGray_, umatGray_ );

	std::vector<cv::Rect> faces;
	face_classifier_.detectMultiScale(umatGray_, faces, 1.1,
		3,
		CV_HAAR_SCALE_IMAGE,
		cv::Size(50,50));
	/*
	#define CV_HAAR_DO_CANNY_PRUNING    1
	#define CV_HAAR_SCALE_IMAGE         2
	#define CV_HAAR_FIND_BIGGEST_OBJECT 4
	#define CV_HAAR_DO_ROUGH_SEARCH     8
	*/

	for(int i=0; i<faces.size(); i++) {
		cv::Point lb(faces[i].x + faces[i].width,
						faces[i].y + faces[i].height);
		cv::Point tr(faces[i].x, faces[i].y);
 
		cv::rectangle(this->matSrc_, lb, tr, cv::Scalar(0,255,0), 3, 4, 0);
	}

	return 0;

}
