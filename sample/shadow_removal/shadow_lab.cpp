#include "shadow_lab.hpp"

void ShadowLab::detectShadow(cv::Mat colorImage, cv::Mat &resultBinary){
	cv::Mat labImage;
	cv::cvtColor(colorImage, labImage, CV_BGR2Lab);
	
	int cols = labImage.cols;
	int rows = labImage.rows;
	int pixelCount = rows * cols;
	

	//calculate mean
	int sum_L = 0, sum_A = 0, sum_B = 0;
	for (int y = 0; y < rows; ++y){
		for (int x = 0; x < cols; ++x){
			sum_L += labImage.at<cv::Vec3b>(y, x)[0];
			sum_A += labImage.at<cv::Vec3b>(y, x)[1];
			sum_B += labImage.at<cv::Vec3b>(y, x)[2];
		}
	}
	double mean_L = sum_L / pixelCount;
	double mean_A = sum_A / pixelCount;
	double mean_B = sum_B / pixelCount;

	
	//calculate standard deviation
	double sumForDeviation = 0;
	for (int y = 0; y < rows; ++y){
		for (int x = 0; x < cols; ++x){
			uchar L = labImage.at<cv::Vec3b>(y, x)[0];
			sumForDeviation += cv::pow((mean_L - L), 2.0);
		}
	}
	//standard deviation
	double stdDeviation = cv::sqrt(sumForDeviation / pixelCount);

	//detect shadow by LAB
	bool isSumOfAB_Big = false;
	
	if (mean_A + mean_B > 256) isSumOfAB_Big = true;

	if (isSumOfAB_Big){
		int baseValue = mean_L - (stdDeviation / 3);
		for (int y = 0; y < rows; ++y){
			for (int x = 0; x < cols; ++x){
				uchar L = labImage.at<cv::Vec3b>(y, x)[0];
				if (L < baseValue){
					//shadow!!!!
					resultBinary.at<uchar>(y, x) = 255;
				}
				else{
					resultBinary.at<uchar>(y, x) = 0;
				}
			}
		}
	}
	else{
		int threshold = 127;
		for (int y = 0; y < rows; ++y){
			for (int x = 0; x < cols; ++x){
				uchar L = labImage.at<cv::Vec3b>(y, x)[0];
				uchar B = labImage.at<cv::Vec3b>(y, x)[2];
				if (L < threshold || B < threshold){
					//shadow!!!!
					resultBinary.at<uchar>(y, x) = 255;
				}
				else{
					resultBinary.at<uchar>(y, x) = 0;
				}
			}
		}

	}

}

