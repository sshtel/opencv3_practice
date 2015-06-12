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


int finishTaskFlag;
int finishedTaskCount(){
	return finishTaskFlag;
}



#define LOOP_COUNT 30

void sobel_thread_CL(void* pParams)
{
	char *filename = (char*)pParams;

	for (int i = 0; i < LOOP_COUNT; ++i){

		cv::Mat img = cv::imread(filename);

		UMat uSrc = img.getUMat(ACCESS_READ);
		UMat uGray;
		UMat uGrad_X, uGrad_Y;
		UMat abs_grad_x, abs_grad_y;
		UMat grad;
		Mat dst;

		GaussianBlur(uSrc, uSrc, Size(3, 3), 0, 0, BORDER_DEFAULT);

		cvtColor(uSrc, uGray, CV_BGR2GRAY);
		cv::Sobel(uGray, uGrad_X, CV_16S, 1, 0, 3);
		cv::Sobel(uGray, uGrad_Y, CV_16S, 0, 1, 3);

		convertScaleAbs(uGrad_X, abs_grad_x);
		convertScaleAbs(uGrad_Y, abs_grad_y);

		addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

		//grad.copyTo(dst);

		{
			//imshow("", dst);
			//cv::waitKey(0);
		}
		//cv::imwrite(BIG_IMAGE_ROOT"\\sobel_out.jpg", dst);
	}

	finishTaskFlag++;
}

void video_thread_CL(void* pParams)
{
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
