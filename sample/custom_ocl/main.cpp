#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <device_ocl.h>
#include <auto_etr.h>

#ifdef WIN32

#include <windows.h>
#include <process.h>
#include <stdlib.h>
#endif

#include <fstream>
#include <iostream>


using namespace cv;
using namespace std;

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
			////////////_beginthread(sobel_thread_CL, 0, filename);
		}

		while (1){
			/////////if (finishedTaskCount() >= THREAD_NUM){ break; }
			Sleep(10);
			if (waitKey(10) >= 0){ break; }
		}
	}
	return;
}

void ocl_test(){
#define ARRAY_SIZE 128

	std::ifstream file("canny.cl");
	std::string kcode(std::istreambuf_iterator<char>(file),
		(std::istreambuf_iterator<char>()));

	cv::ocl::ProgramSource * programSource;
	programSource = new cv::ocl::ProgramSource(kcode.c_str());

	cv::String errorMessage;
	cv::ocl::Program * program;
	program = new cv::ocl::Program(*programSource, NULL, errorMessage);

	cv::ocl::Kernel * kernel;
	kernel = new cv::ocl::Kernel("canny", *program);
	/* I'm stuck here at the args. */

	size_t globalSize[2] = { ARRAY_SIZE, 1 };
	size_t localSize[2] = { ARRAY_SIZE, 1 };
	kernel->run(ARRAY_SIZE, globalSize, localSize, true);

		

}


#define IMAGE_ROOT "..\\..\\..\\images"

#define IMAGE_LENA IMAGE_ROOT"\\lena.jpg"

void kernel_test()
{
	if (!cv::ocl::haveOpenCL())
	{
		cout << "OpenCL is not avaiable..." << endl;
		return;
	}
	cv::ocl::Context context;
	if (!context.create(cv::ocl::Device::TYPE_GPU))
	{
		cout << "Failed creating the context..." << endl;
		return;
	}

	// In OpenCV 3.0.0 beta, only a single device is detected.
	cout << context.ndevices() << " GPU devices are detected." << endl;
	for (int i = 0; i < context.ndevices(); i++)
	{
		cv::ocl::Device device = context.device(i);
		cout << "name                 : " << device.name() << endl;
		cout << "available            : " << device.available() << endl;
		cout << "imageSupport         : " << device.imageSupport() << endl;
		cout << "OpenCL_C_Version     : " << device.OpenCL_C_Version() << endl;
		cout << endl;
	}

	// Select the first device
	cv::ocl::Device(context.device(0));

	// Transfer Mat data to the device
	cv::Mat mat_src = cv::imread(IMAGE_LENA, cv::IMREAD_GRAYSCALE);
	mat_src.convertTo(mat_src, CV_32F, 1.0 / 255);
	cv::UMat umat_src = mat_src.getUMat(cv::ACCESS_READ, cv::USAGE_ALLOCATE_DEVICE_MEMORY);
	cv::UMat umat_dst(mat_src.size(), CV_32F, cv::ACCESS_WRITE, cv::USAGE_ALLOCATE_DEVICE_MEMORY);

	std::ifstream ifs("shift.cl");
	if (ifs.fail()) return;
	std::string kernelSource((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	cv::ocl::ProgramSource programSource(kernelSource);

	// Compile the kernel code
	cv::String errmsg;
	cv::String buildopt = cv::format("-D dstT=%s", cv::ocl::typeToStr(umat_dst.depth())); // "-D dstT=float"
	cv::ocl::Program program = context.getProg(programSource, buildopt, errmsg);

	cv::ocl::Image2D image(umat_src);
	float shift_x = 100.5;
	float shift_y = -50.0;
	cv::ocl::Kernel kernel("shift", program);
	kernel.args(image, shift_x, shift_y, cv::ocl::KernelArg::ReadWrite(umat_dst));

	size_t globalThreads[3] = { mat_src.cols, mat_src.rows, 1 };
	//size_t localThreads[3] = { 16, 16, 1 };
	bool success = kernel.run(3, globalThreads, NULL, true);
	if (!success){
		cout << "Failed running the kernel..." << endl;
		return;
	}

	// Download the dst data from the device (?)
	cv::Mat mat_dst = umat_dst.getMat(cv::ACCESS_READ);

	cv::imshow("src", mat_src);
	cv::imshow("dst", mat_dst);
	cv::waitKey();
}

int main(){
	kernel_test();
	return 0;
}
