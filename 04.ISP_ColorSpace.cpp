#pragma once
#include "Common.h"

int main()
{
	//..\thirdparty\opencv-4.8.0_wnd\sources\samples\data
	// "D:\woochangmin\C++\src\_AI\VisionAPP\thirdparty\opencv-4.8.0_wnd\sources\samples\data\lena.jpg"
	//../KCCImageNet/stinkbug.png
	std::string fileName = "../thirdparty/opencv-4.8.0_wnd/sources/samples/data/lena.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);//흑백으로 이미지(src) 열어주기 1ch

	//RGB는 3CH이기 때문에 각각의 데이터를 담을 배열을 만들어 줘야함.
	uchar* pData = src.data;//data array...start[0,1,2,3,4,5]
	int length = src.total();//data length// 이미지 픽셀 갯수 가로픽셀*세로픽셀 수
	int channels = src.channels();//이미지 ch 갯수


	cv::Mat src_Y = cv::Mat(src.rows, src.cols, CV_8UC1);//Y 공식에 의해 RGB 3ch의 정보 중 Y에 해당하는 정보만 1ch로 표현
	cv::Mat src_Cb = cv::Mat(src.rows, src.cols, CV_8UC1);//Cb 공식에 의해 RGB 3ch의 정보 중 Cb에 해당하는 정보만 1ch로 표현
	cv::Mat src_Cr = cv::Mat(src.rows, src.cols, CV_8UC1);//Cr 공식에 의해 RGB 3ch의 정보 중 Cr에 해당하는 정보만 1ch로 표현


	cv::Mat src_New = cv::Mat(src.rows, src.cols, CV_8UC3);

	cv::Mat src_Diff = cv::Mat(src.rows, src.cols, CV_8UC3);


	for (size_t row = 0; row < src.rows; row++)
	{
		for (size_t col = 0; col < src.cols; col++)
		{
			int index = (row)*src.cols + (col);//mono->각 배열에 접근 할 index를 만들어 주는거임.

			if (channels == 3)//RGB->YCbCr
			{
				int index_B = (row)*src.cols * 3 + (col * 3 + 0);//RGB->B 배열에 접근 할 index를 만들어 주는거임.
				int index_G = (row)*src.cols * 3 + (col * 3 + 1);//RGB->G 배열에 접근 할 index를 만들어 주는거임.
				int index_R = (row)*src.cols * 3 + (col * 3 + 2);//RGB->R 배열에 접근 할 index를 만들어 주는거임.

				int val_B = pData[index_B];
				int val_G = pData[index_G];
				int val_R = pData[index_R];
				//RGB -> Y Cb Cr
				int val_Y = 0.299 * val_R + 0.587 * val_G + 0.114 * val_B;		//RGB ->Y 공식에 의해 RGB 3ch의 정보 중 Y에 해당하는 정보만 1ch로 표현
				int val_Cb = -0.169 * val_R - 0.331 * val_G + 0.500 * val_B;	//RGB ->Cb 공식에 의해 RGB 3ch의 정보 중 Cb에 해당하는 정보만 1ch로 표현
				int val_Cr = 0.500 * val_R - 0.419 * val_G - 0.0813 * val_B;	//RGB ->Cr 공식에 의해 RGB 3ch의 정보 중 Cr에 해당하는 정보만 1ch로 표현



				src_Y.data[index] = (uchar)val_Y;//uchar로 형변환해서 src_Y이미지로 표현
				src_Cb.data[index] = (uchar)val_Cb;//uchar로 형변환해서 src_Cb이미지로 표현
				src_Cr.data[index] = (uchar)val_Cr;//uchar로 형변환해서 src_Cr이미지로 표현
				//Y Cb Cr      -> RGB 	
				int val_NR = 1.000 * val_Y + 1.402 * val_Cr + 0.000 * val_Cb;			//Y Cb Cr->R 공식에 의해 RGB 3ch의 정보 중 Y에 해당하는 정보를 3ch 중 하나의 ch 값에 넣어줌
				int val_NG = 1.000 * val_Y - 0.714 * val_Cr - 0.344 * val_Cb;			//Y Cb Cr->G 공식에 의해 RGB 3ch의 정보 중 Cb에 해당하는 정보를 3ch 중 하나의 ch 값에 넣어줌
				int val_NB = 1.000 * val_Y + 0.000 * val_Cr + 1.772 * val_Cb;			//Y Cb Cr->B 공식에 의해 RGB 3ch의 정보 중 Cr에 해당하는 정보를 3ch 중 하나의 ch 값에 넣어줌


				src_New.data[index_B] = (uchar)val_NB;
				src_New.data[index_G] = (uchar)val_NG;
				src_New.data[index_R] = (uchar)val_NR;

			}
		}
	}
	src_Diff = src - src_New;

	return 1;
}