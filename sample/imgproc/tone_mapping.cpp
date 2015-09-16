#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

#include <opencv2/photo.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


/*
ref tone map : http://docs.opencv.org/3.0-beta/modules/photo/doc/hdr_imaging.html
ref HDR : http://docs.opencv.org/master/d3/db7/tutorial_hdr_imaging.html

color constancy qna : http://answers.opencv.org/question/9054/color-constancy-in-different-illumination-condition/
color constancy gray world algorithm : http://www.codeproject.com/Articles/653355/Color-Constancy-Gray-World-Algorithm


*/

using namespace cv;

void loadExposureSeq(std::string, std::vector<Mat>&, std::vector<float>&);

void hdr()
{
	std::string path = ".\\"; 

	std::vector<Mat> images;
	std::vector<float> times;
	loadExposureSeq(path, images, times);

	Mat response;
	Ptr<CalibrateDebevec> calibrate = createCalibrateDebevec();
	calibrate->process(images, response, times);

	Mat hdr;
	Ptr<MergeDebevec> merge_debevec = createMergeDebevec();
	merge_debevec->process(images, hdr, times, response);

	Mat ldr;
	Ptr<TonemapDurand> tonemap = createTonemapDurand(2.2f);
	tonemap->process(hdr, ldr);

	Mat fusion;
	Ptr<MergeMertens> merge_mertens = createMergeMertens();
	merge_mertens->process(images, fusion);

	imwrite("fusion.png", fusion * 255);
	imwrite("ldr.png", ldr * 255);
	imwrite("hdr.hdr", hdr);

	return;
}

void loadExposureSeq(std::string path, std::vector<Mat>& images, std::vector<float>& times)
{
	std::ifstream list_file((path + "/list.txt"));
	std::string name;
	float val;
	while (list_file >> name >> val)
	{
		images.push_back(imread(path + name));
		times.push_back(1.0 / val);
	}
}