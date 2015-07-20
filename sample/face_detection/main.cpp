#include <FaceData.hpp>
#include <FaceDetectorCpu.hpp>
#include <FaceDetectorCL.hpp>
#include <device_ocl.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#ifdef WIN32
#include "WorkThreadWin32.hpp"
#include <windows.h>
#include <process.h>
#include <stdlib.h>
#endif

#include <auto_etr.h>

using namespace cv;
using namespace std;


#define THREAD_NUM 4

#define IMAGE_ROOT "..\\..\\..\\images"
#define VIDEO_VGA		IMAGE_ROOT"\\640x480_vga.mp4"
#define VIDEO_1080p		IMAGE_ROOT"\\1080p.mp4"
#define VIDEO_INPUT		VIDEO_VGA


void camera_work(bool isCL){

	FaceDetector *detector;
	if (isCL) { detector = new FaceDetectorCL("face_single_CL");	}
	else{ detector = new FaceDetectorCpu("face_single_cpu"); }
	
	//HAAR_EYE_TREE_EYEGLASSES_DATA
	//HAAR_EYE_DATA
	//HAAR_FRONT_FACE_DEFAULT_DATA
	//LBP_FRONTAL_FACE
	//LBP_PROFILE_FACE
	detector->load(HAAR_FRONT_FACE_DEFAULT_DATA);

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
			
			detector->setSrcImg(frame, 1);
			detector->doWork();
			cv::imshow("result", detector->resultMat());
			
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

void image_work(bool isCL){
	
	cv::Mat img = cv::imread(FACE_IMG_04);

	FaceDetector *detector;
	if (isCL) {
		detector = new FaceDetectorCL("face_single_CL");
	}
	else{
		detector = new FaceDetectorCpu("face_single_cpu");
	}
	
	cv::Mat matDst;
	detector->load(HAAR_FRONT_FACE_DEFAULT_DATA);
	detector->setSrcImg(img, 1);
	detector->doWork();
	cv::imshow("result", detector->resultMat());
	cv::waitKey(0);
}

void video_work_tapi(bool isCL){
	
	int start_t, end_t;
	{
		AutoETR autoETR("video work", start_t, end_t);
		setThreadUseCL(isCL);

		int count = THREAD_NUM;
		std::vector<FaceDetector*> faceDetector;

		for (int i = 0; i < count; i++){
			std::string str = "face";
			char num[5];
			_itoa(i, num, 10);
			str += num;
			FaceDetector *detector;

			bool isCL = isThreadUseCL();
			if (isCL) { detector = new FaceDetectorCL(str.c_str()); }
			else{ detector = new FaceDetectorCpu(str.c_str()); }

			detector->setVideoFile(VIDEO_1080p);
			faceDetector.push_back(detector);
			_beginthread(video_thread_CL, 0, detector);
		}

		while (1){
			Sleep(10);
			if (finishedTaskCount() >= THREAD_NUM) { break; }
			if (waitKey(10) >= 0){ break; }
		}
	}
	std::cout << "start time : " << start_t << std::endl;
	std::cout << "end time : " << end_t << std::endl;

	return;
}




int main(int argc, char *argv[]){
	bool clDeviceFound = true;

	int deviceType = cv::ocl::Device::TYPE_GPU;
	int vendor = cv::ocl::Device::VENDOR_AMD;
	cv::String vendorName;
	DeviceOcl devOcl;
	clDeviceFound = devOcl.checkDefaultDevice(vendor, deviceType);

	if(0){
		cv::ocl::setUseOpenCL(false);  //for CL OFF test
		clDeviceFound = false;
	}
	setImgShow(false);
	//video_work_tapi(clDeviceFound);
	
	//image_work(clDeviceFound);
	camera_work(clDeviceFound);
	
	return 0;
}
