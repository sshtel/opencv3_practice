#include <iostream>
#include <opencv2/opencv.hpp>
#include <device_ocl.h>

#include "pedestrian_detector.h"

using namespace cv;
using namespace std;


#define IMAGE_ROOT "..\\..\\..\\images"

#define HOG_IMG_01 IMAGE_ROOT"\\pedestrian\\1mp\\resize_P6213168.jpg"
#define HOG_IMG_02 IMAGE_ROOT"\\pedestrian\\1mp\\resize_P6213169.jpg"
#define HOG_IMG_03 IMAGE_ROOT"\\pedestrian\\1mp\\resize_P6213170.jpg"
#define HOG_IMG_04 IMAGE_ROOT"\\pedestrian\\1mp\\resize_P6213171.jpg"
#define HOG_IMG_05 IMAGE_ROOT"\\pedestrian\\1mp\\resize_P6213172.jpg"
#define HOG_IMG_06 IMAGE_ROOT"\\pedestrian\\1mp\\resize_P6213173.jpg"
#define HOG_IMG_07 IMAGE_ROOT"\\pedestrian\\1mp\\resize_P6213226.jpg"


#define HOG_VID_01 IMAGE_ROOT"\\pedestrian\\pedestrian01.mp4"
#define HOG_VID_02 IMAGE_ROOT"\\pedestrian\\pedestrian02.mp4"
#define HOG_VID_03 IMAGE_ROOT"\\pedestrian\\pedestrian03.mp4"
#define HOG_VID_04 IMAGE_ROOT"\\pedestrian\\pedestrian04.mp4"
#define HOG_VID_05 IMAGE_ROOT"\\pedestrian\\pedestrian05.mp4"
#define HOG_VID_06 IMAGE_ROOT"\\pedestrian\\pedestrian06.mp4"
#define HOG_VID_07 IMAGE_ROOT"\\pedestrian\\pedestrian07.mp4"
#define HOG_VID_08 IMAGE_ROOT"\\pedestrian\\pedestrian08.mp4"




bool running = true;
String vdo_source = HOG_VID_02;
//String vdo_source = HOG_VID_08;
int camera_id = -1;
String img_source = HOG_IMG_03;
String output = "out";


int main(){

	PedestrianDetector pDetectgor;
	pDetectgor.setSVMDetector(PedestrianDetector::DAIMLER_DETECTOR);

	cv::namedWindow("opencv_hog", 1);

	Mat img_to_show;

	while (running)
	{
		VideoCapture vc;
		UMat frame;

		if (vdo_source != "")
		{
			vc.open(vdo_source.c_str());
			if (!vc.isOpened())
				throw runtime_error(string("can't open video file: " + vdo_source));
			vc >> frame;
		}
		else if (camera_id != -1)
		{
			vc.open(camera_id);
			if (!vc.isOpened())
			{
				stringstream msg;
				msg << "can't open camera: " << camera_id;
				throw runtime_error(msg.str());
			}
			vc >> frame;
		}
		else
		{
			imread(img_source).copyTo(frame);
			if (frame.empty())
				throw runtime_error(string("can't open image file: " + img_source));
		}

		// Iterate over all frames
		while (running && !frame.empty())
		{
			{
				vector<Rect> found;
				pDetectgor.detect(frame, found);
				frame.copyTo(img_to_show);
				// Draw positive classified windows
				for (size_t i = 0; i < found.size(); i++)
				{
					Rect r = found[i];
					rectangle(img_to_show, r.tl(), r.br(), Scalar(0, 255, 0), 3);
				}

				putText(img_to_show, ocl::useOpenCL() ? "Mode: OpenCL" : "Mode: CPU", Point(5, 25), FONT_HERSHEY_SIMPLEX, 1., Scalar(255, 100, 0), 2);
				//putText(img_to_show, "FPS (HOG only): " + hogWorkFps(), Point(5, 65), FONT_HERSHEY_SIMPLEX, 1., Scalar(255, 100, 0), 2);
				//putText(img_to_show, "FPS (total): " + workFps(), Point(5, 105), FONT_HERSHEY_SIMPLEX, 1., Scalar(255, 100, 0), 2);
				imshow("opencv_hog", img_to_show);

				if (cv::waitKey(10) >= 0){
					vc.release();
					break;
				}
				if (vdo_source != "" || camera_id != -1) { vc >> frame; }

			}// work end


		}
	}

	return 0;
}