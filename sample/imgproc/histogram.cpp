#include "histogram.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

using namespace cv;

#define IMAGE_ROOT "..\\..\\..\\images"



#define IMAGE_ROOT "..\\..\\..\\images"
#define BIG_IMAGE_ROOT "..\\..\\..\\images\\big"

#define FACE_IMG_01 IMAGE_ROOT"\\face01.jpg"
#define FACE_IMG_02 IMAGE_ROOT"\\face02.jpg"
#define FACE_IMG_03 IMAGE_ROOT"\\face03.jpg"
#define FACE_IMG_04 IMAGE_ROOT"\\face04.jpg"
#define FACE_IMG_LENA IMAGE_ROOT"\\lena.jpg"


void hist_sample(){

	Mat src_base, hsv_base;
	Mat src_test1, hsv_test1;
	Mat src_test2, hsv_test2;
	Mat hsv_half_down;

	src_base = imread(FACE_IMG_01, 1);
	src_test1 = imread(FACE_IMG_02, 1);
	src_test2 = imread(FACE_IMG_03, 1);

	/// Convert to HSV
	cv::cvtColor(src_base, hsv_base, cv::COLOR_BGR2HSV);
	cv::cvtColor(src_test1, hsv_test1, cv::COLOR_BGR2HSV);
	cv::cvtColor(src_test2, hsv_test2, cv::COLOR_BGR2HSV);

	hsv_half_down = hsv_base(Range(hsv_base.rows / 2, hsv_base.rows - 1), Range(0, hsv_base.cols - 1));

	/// Using 50 bins for hue and 60 for saturation
	int h_bins = 50; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };

	// hue varies from 0 to 179, saturation from 0 to 255
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };

	const float* ranges[] = { h_ranges, s_ranges };

	// Use the o-th and 1-st channels
	int channels[] = { 0, 1 };


	/// Histograms
	MatND hist_base;
	MatND hist_half_down;
	MatND hist_test1;
	MatND hist_test2;

	/// Calculate the histograms for the HSV images
	calcHist(&hsv_base, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false);
	normalize(hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&hsv_half_down, 1, channels, Mat(), hist_half_down, 2, histSize, ranges, true, false);
	normalize(hist_half_down, hist_half_down, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&hsv_test1, 1, channels, Mat(), hist_test1, 2, histSize, ranges, true, false);
	normalize(hist_test1, hist_test1, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&hsv_test2, 1, channels, Mat(), hist_test2, 2, histSize, ranges, true, false);
	normalize(hist_test2, hist_test2, 0, 1, NORM_MINMAX, -1, Mat());


	/// Apply the histogram comparison methods
	for (int i = 0; i < 4; i++)
	{
		int compare_method = i;
		double base_base = compareHist(hist_base, hist_base, compare_method);
		double base_half = compareHist(hist_base, hist_half_down, compare_method);
		double base_test1 = compareHist(hist_base, hist_test1, compare_method);
		double base_test2 = compareHist(hist_base, hist_test2, compare_method);

		printf(" Method [%d] Perfect, Base-Half, Base-Test(1), Base-Test(2) : %f, %f, %f, %f \n", i, base_base, base_half, base_test1, base_test2);
	}

	printf("Done \n");

	return;
}

void histEqualizeYCbCr(cv::UMat &src, cv::UMat &dst){

	Mat ycrcb;
	std::vector<Mat> channels;

	cvtColor(src, ycrcb, CV_BGR2YCrCb);
	split(ycrcb, channels);
	equalizeHist(channels[0], channels[0]);
	merge(channels, ycrcb);
	cvtColor(ycrcb, dst, CV_YCrCb2BGR);

}



void histEqualizeSplitChannels(cv::UMat &src, cv::UMat &dst){

	std::vector<Mat> channels;
	split(src, channels);
	equalizeHist(channels[0], channels[0]);
	equalizeHist(channels[1], channels[1]);
	equalizeHist(channels[2], channels[2]);
	merge(channels, dst);

	
}


void hist_equ_sample(){

#define FACE_SAMPLE_1 IMAGE_ROOT"\\face01.jpg"
#define FACE_SAMPLE_1_DARK IMAGE_ROOT"\\face01_dark.jpg"
#define FACE_SAMPLE_1_BRIGHT IMAGE_ROOT"\\face01_bright.jpg"
	UMat srcUmat, dstUmat;
	
	srcUmat = imread(FACE_SAMPLE_1, 1).getUMat(cv::ACCESS_READ);
	dstUmat = cv::UMat(srcUmat);
	cv::imshow("face01", srcUmat);
	cv::waitKey(0);
	histEqualizeSplitChannels(srcUmat, dstUmat);
	cv::imshow("face01 eq", dstUmat);
	cv::waitKey(0);

}






/*
refer to:
https://en.wikipedia.org/wiki/Adaptive_histogram_equalization
http://docs.opencv.org/master/d5/daf/tutorial_py_histogram_equalization.html
*/

void cleah(cv::UMat &src, cv::UMat &dst){
	cv::Ptr<CLAHE> clahe = cv::createCLAHE(1.0, cv::Size(2, 2));
	Mat ycrcb;
	std::vector<Mat> channels;
	cvtColor(src, ycrcb, CV_BGR2YCrCb);
	split(ycrcb, channels);
	clahe->apply(channels[0], channels[0]);
	merge(channels, ycrcb);
	cvtColor(ycrcb, dst, CV_YCrCb2BGR);
}



void clahe_sample(){

#define FACE_SAMPLE_1 IMAGE_ROOT"\\face01.jpg"
#define FACE_SAMPLE_1_DARK IMAGE_ROOT"\\face01_dark.jpg"
#define FACE_SAMPLE_1_BRIGHT IMAGE_ROOT"\\face01_bright.jpg"
	UMat srcUmat, dstUmat;

	srcUmat = imread(FACE_SAMPLE_1, 1).getUMat(cv::ACCESS_READ);
	dstUmat = cv::UMat(srcUmat);
	cv::imshow("face01", srcUmat);
	cv::waitKey(0);
	cleah(srcUmat, dstUmat);
	cv::imshow("face01 eq", dstUmat);
	cv::waitKey(0);

	srcUmat = imread(FACE_SAMPLE_1_DARK, 1).getUMat(cv::ACCESS_READ);
	dstUmat = cv::UMat(srcUmat);
	cv::imshow("face01 dark", srcUmat);
	cv::waitKey(0);
	cleah(srcUmat, dstUmat);
	cv::imshow("face01 dark eq", dstUmat);
	cv::waitKey(0);

	srcUmat = imread(FACE_SAMPLE_1_BRIGHT, 1).getUMat(cv::ACCESS_READ);
	dstUmat = cv::UMat(srcUmat);
	cv::imshow("face01 bright", srcUmat);
	cv::waitKey(0);
	cleah(srcUmat, dstUmat);
	cv::imshow("face01 bright eq", dstUmat);
	cv::waitKey(0);

}

void gamma_correction_sample(){
#define FACE_SAMPLE_1 IMAGE_ROOT"\\face01.jpg"
	UMat srcUmat, dstUmat;

	srcUmat = imread(FACE_SAMPLE_1, 1).getUMat(cv::ACCESS_READ);
	dstUmat;
	cv::imshow("face01", srcUmat);
	cv::waitKey(0);
	cv::pow(srcUmat, 1, dstUmat);
	cv::imshow("face01 gamma 1.0/1.0", dstUmat);
	cv::waitKey(0);
	
	cv::pow(srcUmat, 1 / 2.5, dstUmat);  //error
	cv::imshow("face01 gamma 1.0/2.2", dstUmat);
	cv::waitKey(0);

	cv::pow(srcUmat, 1.0/2.0, dstUmat);
	cv::imshow("face01 gamma 1.0/2.0", dstUmat);
	cv::waitKey(0);
}



/*
//http://docs.opencv.org/doc/tutorials/core/basic_linear_transform/basic_linear_transform.html
* Enter the alpha value [1.0-3.0]
* Enter the beta value [0-100]
*/
void basicLinearTransforms(Mat srcmat, Mat dstmat, double alpha, int beta){
	/// Do the operation new_image(i,j) = alpha*image(i,j) + beta
	for (int y = 0; y < srcmat.rows; y++)
	{
		for (int x = 0; x < srcmat.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				dstmat.at<Vec3b>(y, x)[c] =
					saturate_cast<uchar>(alpha*(dstmat.at<Vec3b>(y, x)[c]) + beta);
			}
		}
	}

}

void brightness_sample(){
#define FACE_SAMPLE_1 IMAGE_ROOT"\\face01.jpg"
	Mat src;
	Mat dst;
	src = imread(FACE_SAMPLE_1, 1);
	cv::imshow("face01", src);
	cv::waitKey(0);

	dst = Mat(src);
	basicLinearTransforms(src, dst, 1.2, 0);
	cv::imshow("face01 bright 2.2", dst);
	cv::waitKey(0);


	dst = Mat(src);
	basicLinearTransforms(src, dst, 1.5, 0);
	cv::imshow("face01 bright 3.3", dst);
	cv::waitKey(0);

}



//Balancing contrast and brightness between stitched images??
//http://stackoverflow.com/questions/13978689/balancing-contrast-and-brightness-between-stitched-images

//Exposure Compensator
//http://docs.opencv.org/modules/stitching/doc/exposure_compensation.html