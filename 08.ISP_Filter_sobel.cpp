//sovel HPF filter
#pragma once

#include "ISP.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	Mat soble_x = Mat::zeros(src_gray.size(), CV_8UC1);
	Mat soble_y = Mat::zeros(src_gray.size(), CV_8UC1);
	Mat sobel_edge = Mat::zeros(src_gray.size(), CV_8UC1);

	//filter 3x3 ... 1/9
	const int filter_sz = 3;
	//float blur[] = {
	//	1.0 / 9, 1.0 / 9, 1.0 / 9,
	//	1.0 / 9, 1.0 / 9, 1.0 / 9,
	//	1.0 / 9, 1.0 / 9, 1.0 / 9,
	//};
	int sobel_x[] = {
	-1, -2, -1,
	0, 0, 0,
	1, 2, 1,
	};
	int sobel_y[] = {
	1, 0, -1,
	2, 0, -2,
	1, 0, -1,
	};

	int threshold = 80;
	int half_kernelSize = filter_sz / 2;
	for (size_t row = half_kernelSize; row < src_gray.rows - half_kernelSize; row++)
	{
		for (size_t col = half_kernelSize; col < src_gray.cols - half_kernelSize; col++)
		{
			int grad_x = 0;
			int grad_y = 0;
			for (int f_row = -half_kernelSize; f_row <= half_kernelSize; f_row++)
			{
				for (int f_col = -half_kernelSize; f_col <= half_kernelSize; f_col++)
				{
					int index = (row + f_row) * src_gray.cols + (col + f_col);
					int f_index = (f_row + half_kernelSize) * filter_sz + (f_col + half_kernelSize);
					grad_x += src_gray.data[index] * sobel_x[f_index];
					grad_y += src_gray.data[index] * sobel_y[f_index];
				}
			}
			int index = (row)*src_gray.cols + (col);
			soble_x.data[index] = std::abs(grad_x);
			soble_y.data[index] = std::abs(grad_y);

			sobel_edge.data[index] = (unsigned char)sqrt((double)(grad_x * grad_x + grad_y * grad_y));//td::abs(grad_x) + std::abs(grad_y);
			//if (sobel_edge.data[index] >= threshold)
			//	sobel_edge.data[index] = 255;
			//else
			//	sobel_edge.data[index] = 0;

		}
	}


	//copy col+1 to col, col[1]->col[0], left
	for (size_t row = 0; row < sobel_edge.rows; row++)
	{
		int index_0 = row * sobel_edge.cols + 0;
		int index_1 = row * sobel_edge.cols + 1;
		sobel_edge.data[index_0] = sobel_edge.data[index_1];

		index_0 = row * sobel_edge.cols + (sobel_edge.cols - 1);
		index_1 = row * sobel_edge.cols + (sobel_edge.cols - 2);
		sobel_edge.data[index_0] = sobel_edge.data[index_1];
	}



	//copy col to col+1, col[98]->col[99], right
	for (size_t col = 0; col < sobel_edge.cols; col++)
	{
		int index_0 = (0) * sobel_edge.cols + col;
		int index_1 = (1) * sobel_edge.cols + col;
		sobel_edge.data[index_0] = sobel_edge.data[index_1];

		index_0 = (sobel_edge.rows - 1) * sobel_edge.cols + col;
		index_1 = (sobel_edge.rows - 2) * sobel_edge.cols + col;
		sobel_edge.data[index_0] = sobel_edge.data[index_1];
	}

	//copy row+1 to row, row[1]->row[0], top
	//copy row to row+1, row[98]->row[99], bottom
	//corner[4]... 좌상, (col[1]+row[1])/2

	// Sobel Filter    
	{
		Mat mag;// = Mat::zeros(src_gray.size(), CV_16S);
		Mat dx, dy;
		Sobel(src_gray, dx, CV_32FC1, 1, 0);
		Sobel(src_gray, dy, CV_32FC1, 0, 1);
		magnitude(dx, dy, mag);
		mag.convertTo(mag, CV_8UC1);
		Mat edge = mag >= threshold;
		int a = 0;
	}

	return 1;
}