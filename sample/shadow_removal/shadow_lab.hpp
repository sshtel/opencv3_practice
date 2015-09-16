#ifndef __SHADOW_LAB_H
#define __SHADOW_LAB_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>


/*
references:

IJCSI-10-4-2-270-273_A Shadow Detection and Removal from a Single Image Using LAB Color Space
S_Murali,V_Govindan_Shadow Detection and Removal from a Single Image Using LAB Color Space
*/


class ShadowLab{
	
public:
	void detectShadow(cv::Mat colorImage, cv::Mat &resultBinary);
	
};


#endif