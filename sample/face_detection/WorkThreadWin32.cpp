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

using namespace std;
using namespace cv;


#ifdef WIN32

bool threadUseCL;
void setThreadUseCL(bool isCL){
	threadUseCL = isCL;
}
bool isThreadUseCL(){ return threadUseCL; }

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
	faceDetector->load(HAAR_EYE_TREE_EYEGLASSES_DATA);

	VideoCapture videoCapture;
	cv::Mat frame, frameCopy, image;

	videoCapture.open(faceDetector->videoFile().c_str());
	if (!videoCapture.isOpened()) { cout << "No video detected" << endl; return; }

	cv::namedWindow(name.c_str(), 1);

	unsigned int count = 0;

	if (videoCapture.isOpened())
	{
		cout << "In capture ..." << name.c_str() << endl;
		for (;;)
		{
			count++;
			if (!videoCapture.grab()) continue;
			videoCapture.retrieve(frame, 0);

			faceDetector->setSrcImg(frame, 1);
			faceDetector->doWork();
			cv::imshow(name.c_str(), faceDetector->resultMat());

			if (waitKey(10) >= 0){
				videoCapture.release();
				break;
			}

			Sleep(1);
		}
	}
	waitKey(0);

	cvDestroyWindow(name.c_str());
	_endthread();
	return;

}


#endif
