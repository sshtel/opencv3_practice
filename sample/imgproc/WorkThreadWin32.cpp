#include "WorkThreadWin32.hpp"

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
	
	//HAAR_EYE_TREE_EYEGLASSES_DATA
	//HAAR_EYE_DATA
	//HAAR_FRONT_FACE_DEFAULT_DATA
	//LBP_FRONTAL_FACE
	//LBP_PROFILE_FACE
	
	VideoCapture videoCapture;
	cv::Mat frame, frameCopy, image;

	if (!videoCapture.isOpened()) { cout << "No video detected" << endl; return; }

	//cv::namedWindow(name.c_str(), 1);

	unsigned int count = 0;
	std::vector<cv::Rect> faces_result;
	if (videoCapture.isOpened())
	{
		for (;;)
		{
			count++;
			if (!videoCapture.grab()) { break; }
			if (!videoCapture.retrieve(frame, 0)) { break; }

			

			if (waitKey(10) >= 0){
				videoCapture.release();
				break;
			}

			Sleep(1);
		}
	}
	
	//cvDestroyWindow(name.c_str());
	_endthread();
	return;

}


#endif
