#pragma once

#include "ISP.h"

int main()
{

	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);

	uchar* pData = src.data;//color pointer
	int length = src.total();//data length=cols*rows
	int channels = src.channels();
	const int histoSz = 256;
	cv::Mat gray_cv = cv::Mat(src.rows, src.cols, CV_8UC1);
	cv::Mat gray = cv::Mat(src.rows, src.cols, CV_8UC1);

	cv::cvtColor(src, gray_cv, ColorConversionCodes::COLOR_BGR2GRAY);
	ISP _isp;
	Mat diff = gray_cv - gray;
	cv::Mat gray_Eq = cv::Mat(src.rows, src.cols, CV_8UC1);


	_isp.Convert_BGR2GRAY(src.data, src.cols, src.rows, gray.data);
	_isp.Enhance_HistogramEq(gray.data,	gray.cols,gray.rows,gray_Eq.data);
	
	//Using library OpenCV
	//check correlation
	{
		Mat gray_cv;
		Mat gray_Eq_cv;
		cvtColor(src, gray_cv, COLOR_BGR2GRAY);
		cv::equalizeHist(gray_cv, gray_Eq_cv);

		Mat diff = gray_cv - gray_Eq_cv;
		double error = cv::sum(diff)[0];
		int a = 0;
	}




	return 1;
}