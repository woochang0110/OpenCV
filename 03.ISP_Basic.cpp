#pragma once
#include "Common.h"

int main()
{
	std::string fileName = "../KCCImageNet/stinkbug.png";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);//흑백으로 이미지(src) 열어주기 1ch
	cv::resize(src, src, cv::Size(src.cols * 2, src.rows * 2));//이미지 2배로 뻥튀기 cols세로2배  rows가로2배
	int datas[10] = { 5,3,1,4,6,4,8,4,3 };
	int* pDatas = datas;//data array

	uchar* pData = src.data;//data array...start[0,1,2,3,4,5]
	int length = src.total();//data length// 이미지 픽셀 갯수 가로픽셀*세로픽셀 수
	int channels = src.channels();//이미지 ch 갯수
	//length *= channels;

	//HISTOGRAM: 도수분포를 나타내는 그래프

	const int histoSz = 256;//sizeof(uchar);
	int histo[histoSz] = { 0, };//HISTO 밝기 빈도수 
	//int* pHisto = new int[length];

	for (size_t i = 0; i < length; i++)
	{
		histo[pData[i]]++;
	}

	uchar* data = src.data;
	int size = src.rows * src.cols;//행(가로)*열(세로)
	auto result = std::minmax_element(pData, pData + length);
	double mean_brightness = cv::mean(src)[0];//mean은 배열의 평균값을 구해주는 함수
	auto _minmax = std::minmax_element(histo, histo + histoSz);//pair(int 2개) 리턴해줌  
	int _min_value = *_minmax.first;//pair(int 2개 중 첫번째) 리턴해줌  
	int _max_value = *_minmax.second;//pair(int 2개중 두번째) 리턴해줌  

	//=========도스 창에 히스토그램 표현하기
	std::string strLine;
	for (size_t row = 0; row < histoSz; row++)
	{
		strLine = "";
		strLine += to_string(row);
		strLine += ("   ||");

		int scaled_cols = (histo[row] * 1.0 / _max_value) * 100;
		for (size_t col = 0; col < scaled_cols; col++)
		{
			strLine += ("=");
		}
		std::cout << strLine << std::endl;
	}
	//=========도스 창에 히스토그램 표현하기 끝


	//=========새롭게 이미지(draw) 하나 만들어서 이미지 위에 히스토그램 표현하기
	cv::Mat draw = src.clone();//draw<- src 이미지 복사
	cvtColor(draw, draw, ColorConversionCodes::COLOR_GRAY2BGR);//draw 이미지를 여는데 color를 흑백에서 BGR로 변환해주고
	for (size_t row = 0; row < histoSz; row++)//cols세로  rows가로
	{
		int scaled_cols = (histo[row] * 1.0 / _max_value) * src.cols;
		cv::Point st = Point(0, row * 2.7);//그냥 row 값으로 하니까 히스토그램이 위쪽에서만 출력되고 끝나서 이미지에 맞게 가운데 쪽 값들이 가운데에 출력되도록 2.7을 곱해주는거임
		cv::Point ed = Point(scaled_cols, row * 2.7);//그냥 row 값으로 하니까 히스토그램이 위쪽에서만 출력되고 끝나서 이미지에 맞게 가운데 쪽 값들이 가운데에 출력되도록 2.7을 곱해주는거임
		cv::line(draw, st, ed, CV_RGB(255, 255, 0));
	}
	//=========새롭게 이미지(draw) 하나 만들어서 이미지 위에 히스토그램 표현하기

	return 1;

}