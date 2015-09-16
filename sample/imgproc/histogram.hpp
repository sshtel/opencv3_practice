#ifndef __HISTOGRAM__H
#define __HISTOGRAM__H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

void hist_sample();
void hist_equ_sample();
void histEqualizeColorImage(cv::UMat &src, cv::UMat &dst);
void clahe_sample();

void gamma_correction_sample();
void brightness_sample();

#endif