#pragma once

#include "ISP.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);
	cv::Mat gray = cv::Mat(src.rows, src.cols, CV_8UC1);
	int length = src.total();//data length=cols*rows
	//int channels = src.channels();

	ISP _isp;
	double gamma = 4;
	Mat src_gamma = Mat(src.size(), CV_8UC1);

	//Gray Input
	_isp.Convert_BGR2GRAY(src.data, src.cols, src.rows, gray.data);

	//Gamma Correction
	for (size_t i = 0; i < length; i++)
	{
		double normalized_value = (double)gray.data[i] / 255.0;
		src_gamma.data[i] = static_cast<uchar>
			(std::pow(gray.data[i] / 255.0, gamma) * 255.0 + 0.5);
	}

	//Gray Output

	return 1;
}