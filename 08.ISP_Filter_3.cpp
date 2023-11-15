#pragma once

#include "ISP.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	cv::Mat src_gray_blur = src_gray.clone();
	src_gray_blur = 0;

	//filter 3x3 ... 1/9
	const int filter_sz = 3;//3x3 filter
	//float blur[] = {
	//	1.0 / 9, 1.0 / 9, 1.0 / 9,
	//	1.0 / 9, 1.0 / 9, 1.0 / 9,
	//	1.0 / 9, 1.0 / 9, 1.0 / 9,
	//};
	//float blur[] = {
	//1.0 / 9, 1.0 / 9, 1.0 / 9,
	//1.0 / 9, 1.0 / 9, 1.0 / 9,
	//1.0 / 9, 1.0 / 9, 1.0 / 9,
	//};
	float blur[] = {	//mask�� ���� �� ������ ����

	1.0, 2.0, 1.0,
	2.0, 4.0, 2.0,
	1.0, 2.0, 1.0,
	};
	for (size_t i = 0; i < filter_sz * filter_sz; i++)
	{
		blur[i] /= 16;
	}
											//                 (-1,-1) (0,-1) (1,-1)
	int half_kernelSize = filter_sz / 2; // filter size�� 3�̸� (-1,0)  (0,0)  (1,0) �� filter�� �������� half_kernelSize�� �ذ�
															//	(-1,1)  (0,1)  (1,1)
	for (size_t row = half_kernelSize; row < src_gray.rows - half_kernelSize; row++)//�� �Ʒ� 1�ȼ����� filter size����ؼ� �����ϰ� ����(1~row~510)
	{
		for (size_t col = half_kernelSize; col < src_gray.cols - half_kernelSize; col++)// �� �� 1�ȼ����� filter size����ؼ� �����ϰ� ����(1~col~510)
		{
			//1pixel�� �������� �ش� pixel ������ ������ filter���� �°� ������ �� ���� ����� ����ϴ� filter �����
			int sum = 0;
			for (int f_row = -half_kernelSize; f_row <= half_kernelSize; f_row++)//-1~f_row~1���� 
			{
				for (int f_col = -half_kernelSize; f_col <= half_kernelSize; f_col++)//-1~f_col~1����
				{
					int index = (row + f_row) * src_gray.cols + (col + f_col);//filter�� -1 0 1 �� ���� index�� �������ְ�
					int f_index = (f_row + half_kernelSize) * filter_sz + (f_col + half_kernelSize);//index�� ���� �� �� ������, -1~1������ index�� 0~8������ ������ ��ȯ
					sum += src_gray.data[index] * blur[f_index];//�ش� �ȼ� ������ ������ filter�� �°� �����ְ� ������. ��ó 3x3��ŭ �� ������
				}
			}
			int index = (row)*src_gray.cols + (col);
			src_gray_blur.data[index] = static_cast<uchar>(sum);//filter�� ��ģ ������ uchar�� ��ȯ�ؼ� src_gray_blur�� �־��ش�.
		}
	}


	//copy col+1 to col, col[1]->col[0], left
	for (size_t row = 0; row < src_gray_blur.rows; row++)//y��0~511
	{
		//���� �׵θ� ���� 
		int index_0 = row * src_gray_blur.cols + 0;//row*512 + 0
		int index_1 = row * src_gray_blur.cols + 1;//row*512 + 1
		src_gray_blur.data[index_0] = src_gray_blur.data[index_1];// ���� (0,row)<-(1,row)
		//������ �׵θ� ����
		index_0 = row * src_gray_blur.cols + (src_gray_blur.cols - 1);//row*512 + 511
		index_1 = row * src_gray_blur.cols + (src_gray_blur.cols - 2);//row*512 + 510
		src_gray_blur.data[index_0] = src_gray_blur.data[index_1];// ������ (511,row) <- (510,row)
	}
	//copy col to col+1, col[98]->col[99], right
	for (size_t col = 0; col < src_gray_blur.cols; col++)//x�� 0~511
	{
		//�� �׵θ� ����
		int index_0 = (0) * src_gray_blur.cols + col;//col
		int index_1 = (1) * src_gray_blur.cols + col;//col+512
		src_gray_blur.data[index_0] = src_gray_blur.data[index_1];//�� (col,0)<-(col,1)

		//�Ʒ� �׵θ� ����
		index_0 = (src_gray_blur.rows - 1) * src_gray_blur.cols + col;//���ϴ� �ȼ�+col
		index_1 = (src_gray_blur.rows - 2) * src_gray_blur.cols + col;//���ϴ� �ٷ� �� �ȼ�+col
		src_gray_blur.data[index_0] = src_gray_blur.data[index_1];//�Ʒ� (col,511)<-(col,510)
	}

	return 1;
}