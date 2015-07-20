#include "WorkThreadWin32.hpp"
#include <FaceData.hpp>
#include <FaceDetectorCpu.hpp>
#include <FaceDetectorCL.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <opencv2/video.hpp>

#include <iostream>
#include <windows.h>
#include <process.h>

#include <auto_etr.h>

using namespace std;
using namespace cv;


#ifdef WIN32

bool threadUseCL;
void setThreadUseCL(bool isCL){	threadUseCL = isCL; }
bool isThreadUseCL(){ return threadUseCL; }

bool imshowFlag = false;
void setImgShow(bool isImgShow){ imshowFlag = isImgShow; }
bool isImgShow() { return imshowFlag; }

int finishTaskFlag;
int finishedTaskCount(){
	return finishTaskFlag;
}

void video_thread_CL(void* pParams)
{
	FaceDetector *faceDetector;
	if (threadUseCL){
		faceDetector = (FaceDetectorCL*)pParams;
	}
	else{
		faceDetector = (FaceDetectorCpu*)pParams;
	}
	
	std::string name = faceDetector->name();

	//HAAR_EYE_TREE_EYEGLASSES_DATA
	//HAAR_EYE_DATA
	//HAAR_FRONT_FACE_DEFAULT_DATA
	//LBP_FRONTAL_FACE
	//LBP_PROFILE_FACE
	faceDetector->load(HAAR_FRONT_FACE_DEFAULT_DATA);

	VideoCapture videoCapture;
	cv::Mat frame, frameCopy, image;

	videoCapture.open(faceDetector->videoFile().c_str());
	if (!videoCapture.isOpened()) { cout << "No video detected" << endl; return; }

	if (imshowFlag) { cv::namedWindow(name.c_str(), 1); }

	if (videoCapture.isOpened())
	{
		cout << "In capture ..." << name.c_str() << endl;
		while (videoCapture.grab())
		{
			if (!videoCapture.retrieve(frame, 0)) { break; }

			faceDetector->setSrcImg(frame, 1);
			faceDetector->doWork();
			if (imshowFlag){ cv::imshow(name.c_str(), faceDetector->resultMat()); }
			
			std::vector<cv::Rect> &faces_result = faceDetector->getResultFaces();
			std::cout << "face --" << name.c_str() << std::endl;
			for (int i = 0; i < faces_result.size(); ++i){
				std::cout << faces_result.at(i).x << ", " << faces_result.at(i).y << std::endl;
			}
			
			if (waitKey(10) >= 0){
				videoCapture.release();
				break;
			}

			Sleep(1);
		}
	}
	
	if (imshowFlag) { cvDestroyWindow(name.c_str()); }
	finishTaskFlag++;
	_endthread();
	return;

}


#endif
