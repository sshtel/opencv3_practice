#include <device_ocl.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#ifdef WIN32
#include <windows.h>
#include <process.h>
#include <stdlib.h>
#endif

using namespace cv;
using namespace std;


#define IMAGE_ROOT "..\\..\\..\\images"


#define POISSON IMAGE_ROOT"\\poisson.jpg"
#define LENA_BLACK IMAGE_ROOT"\\lena_black.jpg"

void seamless_lena(){

	cv::Mat dst;
	cv::UMat umatSrc = cv::imread(LENA_BLACK).getUMat(cv::ACCESS_READ);
	cv::UMat umatDst = cv::imread(POISSON).getUMat(cv::ACCESS_READ);
	cv::UMat umatGray;
	int srcRows = umatSrc.rows;
	int srcCols = umatSrc.cols;

	cv::UMat umatOutput;
	{
		// Define the mask as a closed polygon
		Point poly[1][12];
		poly[0][0] = Point(130, 19);
		poly[0][1] = Point(193, 5);
		poly[0][2] = Point(270, 41);
		poly[0][3] = Point(265, 198);
		poly[0][4] = Point(312, 345);
		poly[0][5] = Point(314, 464);
		poly[0][6] = Point(135, 470);
		poly[0][7] = Point(128, 426);
		poly[0][8] = Point(150, 362);
		poly[0][9] = Point(126, 256);
		poly[0][10] = Point(94, 163);
		poly[0][11] = Point(120, 89);

		const Point* polygons[1] = { poly[0] };
		int num_points[] = { 12 };
		cv::Mat src_mask = Mat::zeros(srcRows, srcCols, umatSrc.depth());

		cv::fillPoly(src_mask, polygons, num_points, 1, Scalar(255, 255, 255));
	}
	
	// Define the mask as a closed polygon
	Point poly[1][7];
	poly[0][0] = Point(130, 19);
	poly[0][1] = Point(193, 5);
	poly[0][2] = Point(270, 41);
	poly[0][3] = Point(312, 345);
	poly[0][4] = Point(314, 464);
	poly[0][5] = Point(150, 362);
	poly[0][6] = Point(120, 89);
	const Point* polygons[1] = { poly[0] };
	int num_points[] = { 7 };
	cv::Mat src_mask = Mat::zeros(srcRows, srcCols, umatSrc.depth());

	cv::fillPoly(src_mask, polygons, num_points, 1, Scalar(255, 255, 255));



	// The location of the center of the src in the dst
	Point center(100, 300);

	seamlessClone(umatSrc, umatDst, src_mask, center, umatOutput, MIXED_CLONE);
	//seamlessClone(umatSrc, umatDst, src_mask, center, umatOutput, NORMAL_CLONE);

	umatOutput.copyTo(dst);
	cv::imshow("test", dst);
	cv::imwrite(IMAGE_ROOT"\\output_seamless_lena.png", dst);

	cv::waitKey(0);
}


#define LENA IMAGE_ROOT"\\lena.jpg"
#define FACE4 IMAGE_ROOT"\\face04.jpg"
void seamless_match(){
	Mat dst;
	cv::UMat umatSrc = cv::imread(LENA).getUMat(cv::ACCESS_READ);
	cv::UMat umatDst = cv::imread(LENA).getUMat(cv::ACCESS_READ);
	cv::UMat umatGray;
	int srcRows = umatSrc.rows;
	int srcCols = umatSrc.cols;

	cv::UMat umatOutput;

	// Define the mask as a closed polygon
	Point poly[1][4];
	poly[0][0] = Point(0, 0);
	poly[0][1] = Point(srcCols, 0);
	poly[0][2] = Point(srcCols, srcRows);
	poly[0][3] = Point(0, srcRows);

	const Point* polygons[1] = { poly[0] };
	int num_points[] = { 4 };
	cv::Mat src_mask = Mat::zeros(srcRows, srcCols, umatSrc.depth());

	cv::fillPoly(src_mask, polygons, num_points, 1, Scalar(255, 255, 255));

	// The location of the center of the src in the dst
	int center_cols  = (srcCols % 2) ? (srcCols / 2) : (srcCols / 2) - 1;
	int center_rows = (srcRows % 2) ? (srcRows / 2) : (srcRows / 2) - 1;
	Point center(center_cols, center_rows);

	seamlessClone(umatSrc, umatDst, src_mask, center, umatOutput, MIXED_CLONE);
	//seamlessClone(umatSrc, umatDst, src_mask, center, umatOutput, NORMAL_CLONE);
	umatOutput.copyTo(dst);
	
	cv::imwrite(IMAGE_ROOT"\\output_seamless_match.png", dst);

	cv::waitKey(0);
}


#define HELICOPTER IMAGE_ROOT"\\helicopter.jpg"
#define LANDSCAPE1 IMAGE_ROOT"\\landscape1.jpg"
#define LANDSCAPE2 IMAGE_ROOT"\\landscape2.jpg"

void seamless_clone(){
	
	cv::Mat dst;
	cv::UMat umatSrc = cv::imread(HELICOPTER).getUMat(cv::ACCESS_READ);
	cv::UMat umatDst = cv::imread(LANDSCAPE2).getUMat(cv::ACCESS_READ);
	cv::UMat umatGray;
	int srcRows = umatSrc.rows;
	int srcCols = umatSrc.cols;

	cv::UMat umatOutput;

	// Define the mask as a closed polygon
	Point poly[1][7];
	poly[0][0] = Point(603, 562);
	poly[0][1] = Point(668, 528);
	poly[0][2] = Point(789, 536);
	poly[0][3] = Point(805, 636);
	poly[0][4] = Point(758, 701);
	poly[0][5] = Point(667, 712);
	poly[0][6] = Point(590, 664);

	const Point* polygons[1] = { poly[0] };
	int num_points[] = { 7 };
	cv::Mat src_mask = Mat::zeros(srcRows, srcCols, umatSrc.depth());

	cv::fillPoly(src_mask, polygons, num_points, 1, Scalar(255, 255, 255));
	
	// The location of the center of the src in the dst
	//Point center(800, 100);
	Point center(150, 150);

	seamlessClone(umatSrc, umatDst, src_mask, center, umatOutput, MIXED_CLONE);
	//seamlessClone(umatSrc, umatDst, src_mask, center, umatOutput, NORMAL_CLONE);

	umatOutput.copyTo(dst);
	cv::imwrite(IMAGE_ROOT"\\output_seamless_clone.png", dst);
	cv::imshow(IMAGE_ROOT"\\output_seamless_clone.png", dst);
	cv::waitKey(0);
}



int main(){

	seamless_match();
	//seamless_clone();
	//seamless_lena();
	
	return 0;
}
