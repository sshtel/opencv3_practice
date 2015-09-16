#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include "shadow_lab.hpp"
#include "common_directory.hpp"

#include <device_ocl.h>
#include <auto_etr.h>


#ifdef WIN32

#include <windows.h>
#include <process.h>
#include <stdlib.h>
#endif


using namespace std;



void camera_work(bool isCL){

	cv::VideoCapture videoCapture;
	cv::Mat frame, frameCopy, image;
	cv::Mat binaryResult;
	ShadowLab shadowLab;
	

	unsigned int count = 0;
	if (!videoCapture.open(2)) { cout << "No camera detected" << endl; return; }

	cv::namedWindow("result", 1);

	bool isStart = false;
	if (videoCapture.isOpened())
	{
		cout << "In capture ..." << endl;
		for (;;)
		{
			count++;
			
			if (!videoCapture.grab()) continue;
			videoCapture.retrieve(frame, 0);
			if (!isStart){
				binaryResult = cv::Mat(frame.rows, frame.cols, CV_8U);
			}
			
			shadowLab.detectShadow(frame, binaryResult);
			cv::imshow("result", binaryResult);
			if (cv::waitKey(10) >= 0){
				videoCapture.release();
				break;
			}
		}
	}
	cv::waitKey(0);

	cvDestroyWindow("result");

	return;
}


void image_work(cv::String imgFile){
	
	cv::Mat img;
	cv::Mat binaryResult;
	int start_t = 0, end_t = 0;
	{
		AutoETR autuETR("shadow_removal", start_t, end_t);
		ShadowLab shadowLab;
		img = cv::imread(imgFile);
		binaryResult = cv::Mat(img.rows, img.cols, CV_8U);
		shadowLab.detectShadow(img, binaryResult);

	}
	std::cout << "start time : " << start_t << std::endl;
	std::cout << "end time : " << end_t << std::endl;

	cv::imshow("original", img);
	cv::imshow("shadow detect", binaryResult);

	cv::waitKey(0);
}

#define IMAGE_ROOT "..\\..\\..\\images"

#define IMG_01 IMAGE_ROOT"\\shadow_removal\\shadow_removal_01.jpg"
#define IMG_02 IMAGE_ROOT"\\shadow_removal\\shadow_removal_02.jpg"
#define IMG_03 IMAGE_ROOT"\\shadow_removal\\shadow_removal_03.jpg"
#define IMG_04 IMAGE_ROOT"\\shadow_removal\\shadow_removal_04.jpg"
#define IMG_05 IMAGE_ROOT"\\shadow_removal\\shadow_removal_05.jpg"
#define IMG_06 IMAGE_ROOT"\\shadow_removal\\shadow_removal_06.jpg"
#define IMG_07 IMAGE_ROOT"\\shadow_removal\\shadow_removal_07.jpg"
#define IMG_08 IMAGE_ROOT"\\shadow_removal\\shadow_removal_08.jpg"
#define IMG_09 IMAGE_ROOT"\\shadow_removal\\shadow_removal_09.jpg"
#define IMG_10 IMAGE_ROOT"\\shadow_removal\\shadow_removal_10.jpg"
#define IMG_11 IMAGE_ROOT"\\shadow_removal\\shadow_removal_11.jpg"
#define IMG_12 IMAGE_ROOT"\\shadow_removal\\2.jpg"


int main(int argc, char *argv[]){


	if (argc < 2){
		image_work(IMG_08);
	}
	else{
		std::string argv_str(argv[1]);
		if (argv_str == "camera") camera_work(true);
		else {
			

			char cCurrentPath[FILENAME_MAX];
			if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
			{
				return errno;
			}
			std::string path;
			path += cCurrentPath;
			path += "\\";
			path += argv_str;
			std::cout << "filepath : " << argv_str.c_str() << std::endl;
			image_work(path.c_str());
		}
	}

	return 0;
}


