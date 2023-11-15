#pragma once

#include "ISP.h"

int main()
{
	int datas[] = { 6,4,8,9,4,4,8,64,4,6,4,8,6,4,11,1,3,1134,5,64,5,64 };
	//노이즈가 없는 대표 신호 값을 정하시오.

	//4, 6, 8
	int size = sizeof(datas) / sizeof(datas[0]);
	std::sort(datas, datas + size);

	std::vector<int> vDatas(datas, datas + size);
	std::sort(vDatas.begin(), vDatas.end());

	int major = datas[size / 2];
	int major_v = vDatas[size / 2];


	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena_gray.jpg";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	cv::Mat src_gray_remover = src_gray.clone();

	for (size_t row = 1; row < src_gray.rows - 1; row++)
	{
		for (size_t col = 1; col < src_gray.cols - 1; col++)
		{
			int index = (row)*src_gray.cols + (col);

			if ((src_gray.data[index] >= 253) ||
				(src_gray.data[index] <= 1))
			{
				//how??
				int v_NN = src_gray.data[(row - 1) * src_gray.cols + (col)];
				int v_SS = src_gray.data[(row + 1) * src_gray.cols + (col)];
				int v_LL = src_gray.data[(row)*src_gray.cols + (col - 1)];
				int v_RR = src_gray.data[(row)*src_gray.cols + (col + 1)];
				int v_NW = src_gray.data[(row - 1) * src_gray.cols + (col - 1)];
				int v_NE = src_gray.data[(row - 1) * src_gray.cols + (col + 1)];
				int v_SW = src_gray.data[(row + 1) * src_gray.cols + (col - 1)];
				int v_SE = src_gray.data[(row + 1) * src_gray.cols + (col + 1)];

				//src_gray_remover.data[index] = (v_NN + v_SS + v_LL + v_RR) / 4;
				src_gray_remover.data[index] = (v_NN + v_SS + v_LL + v_RR + v_NW + v_NE + v_SW + v_SE) / 8;
			}
		}
	}
	Mat diff = src_gray_remover - src_gray;
	return 1;
}
