#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <device_ocl.h>
#include <auto_etr.h>

#ifdef WIN32
#include "WorkThreadWin32.hpp"
#include <windows.h>
#include <process.h>
#include <stdlib.h>
#endif

using namespace cv;
using namespace std;



#define IMAGE_ROOT "..\\..\\..\\images"
#define BIG_IMAGE_ROOT "..\\..\\..\\images\\big"
#define VIDEO_VGA		IMAGE_ROOT"\\640x480_vga.mp4"
#define VIDEO_1080p		IMAGE_ROOT"\\1080p.mp4"
#define VIDEO_INPUT		VIDEO_VGA


void camera_work(bool isCL){

	VideoCapture videoCapture;
    cv::Mat frame,frameCopy, image;

	unsigned int count = 0;
	if(!videoCapture.open(2)) { cout << "No camera detected" << endl; return; }
	
	cv::namedWindow("result", 1 );
    
    if( videoCapture.isOpened() )
    {
        cout << "In capture ..." << endl;
        for(;;)
        {
			count++;

			if( ! videoCapture.grab() ) continue;
			videoCapture.retrieve(frame , 0);
						
            if( waitKey( 10 ) >= 0 ){
				videoCapture.release();
				break;
			}
        }
	}
    waitKey(0);

    cvDestroyWindow("result");

    return ;
}


#define FACE_IMG_01 IMAGE_ROOT"\\face01.jpg"
#define FACE_IMG_02 IMAGE_ROOT"\\face02.jpg"
#define FACE_IMG_03 IMAGE_ROOT"\\face03.jpg"
#define FACE_IMG_04 IMAGE_ROOT"\\face04.jpg"
#define FACE_IMG_LENA IMAGE_ROOT"\\lena.jpg"

#define BIG_HELICOPTER BIG_IMAGE_ROOT"\\helicopter.jpg"
#define BIG_LANDSCAPE1 BIG_IMAGE_ROOT"\\landscape1.jpg"
#define BIG_LANDSCAPE2 BIG_IMAGE_ROOT"\\landscape2.jpg"
#define BIG_LANDSCAPE3 BIG_IMAGE_ROOT"\\landscape3.png"
#define BIG_PIGEONS BIG_IMAGE_ROOT"\\pigeons.jpg"

void image_work(cv::String imgFile){



	int start_t = 0, end_t = 0;
	{
		AutoETR autuETR("img_sobel", start_t, end_t);
		for (int i = 0; i < 30; ++i) { 
	
			cv::Mat img = cv::imread(imgFile);

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
	}
	std::cout << "start time : " << start_t << std::endl;
	std::cout << "end time : " << end_t << std::endl;

}

#define THREAD_NUM 10
void image_work_multitasking(cv::String imgFile){

	char *filename = const_cast<char*>(imgFile.c_str());

	int start_t, end_t;
	{
		AutoETR autoETR("Multitasking time", start_t, end_t);
		for (int i = 0; i<THREAD_NUM; i++){
			std::string str = "Task";
			char num[5];
			_itoa(i, num, 10);
			str += num;
			std::cout << "begin task : " << str.c_str() << std::endl;
			_beginthread(sobel_thread_CL, 0, filename);
		}

		while (1){
			if (finishedTaskCount() >= THREAD_NUM){ break; }
			Sleep(10);
			if (waitKey(10) >= 0){ break; }
		}
	}
	return;
}

int main(){
	DeviceOcl devOcl;
	bool clDeviceFound = devOcl.checkDefaultDevice(cv::ocl::Device::VENDOR_AMD, cv::ocl::Device::TYPE_GPU);

	cv::ocl::setUseOpenCL(true);
	//image_work(BIG_LANDSCAPE1);
	image_work_multitasking(BIG_LANDSCAPE1);
	
	return 0;
}
