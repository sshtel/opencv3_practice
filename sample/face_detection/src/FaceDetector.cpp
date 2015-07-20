#include <FaceDetector.hpp>
#include <FaceData.hpp>

int FaceDetector::doWork(){
	if (dataPath_ == HAAR_FRONT_FACE_DEFAULT_DATA ||
		dataPath_ == LBP_FRONTAL_FACE ||
		dataPath_ == LBP_PROFILE_FACE
		){
		return cutFace();
	}
	else if (dataPath_ == HAAR_EYE_DATA){
		return cutEyes();
	}
	else if (dataPath_ == HAAR_EYE_TREE_EYEGLASSES_DATA){
		return cutEyes();
	}

	return -1;
}

void FaceDetector::getResultFaces(std::vector<cv::Rect> &faces){
	faces.swap(faces_);
}
