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
	float blur[] = {	//mask의 영역 별 곱해줄 값들

	1.0, 2.0, 1.0,
	2.0, 4.0, 2.0,
	1.0, 2.0, 1.0,
	};
	for (size_t i = 0; i < filter_sz * filter_sz; i++)
	{
		blur[i] /= 16;
	}
											//                 (-1,-1) (0,-1) (1,-1)
	int half_kernelSize = filter_sz / 2; // filter size가 3이면 (-1,0)  (0,0)  (1,0) 로 filter를 잡으려고 half_kernelSize를 준거
															//	(-1,1)  (0,1)  (1,1)
	for (size_t row = half_kernelSize; row < src_gray.rows - half_kernelSize; row++)//위 아래 1픽셀씩은 filter size고려해서 제외하고 블러링(1~row~510)
	{
		for (size_t col = half_kernelSize; col < src_gray.cols - half_kernelSize; col++)// 좌 우 1픽셀씩은 filter size고려해서 제외하고 블러링(1~col~510)
		{
			//1pixel이 정해졌고 해당 pixel 주위에 값들을 filter값에 맞게 곱해준 후 더해 평균을 계산하는 filter 적용부
			int sum = 0;
			for (int f_row = -half_kernelSize; f_row <= half_kernelSize; f_row++)//-1~f_row~1까지 
			{
				for (int f_col = -half_kernelSize; f_col <= half_kernelSize; f_col++)//-1~f_col~1까지
				{
					int index = (row + f_row) * src_gray.cols + (col + f_col);//filter의 -1 0 1 에 맞춰 index을 변경해주고
					int f_index = (f_row + half_kernelSize) * filter_sz + (f_col + half_kernelSize);//index가 음수 일 수 없으니, -1~1사이의 index를 0~8까지의 값으로 변환
					sum += src_gray.data[index] * blur[f_index];//해당 픽셀 주위의 값들을 filter에 맞게 곱해주고 더해줌. 근처 3x3만큼 다 더해줌
				}
			}
			int index = (row)*src_gray.cols + (col);
			src_gray_blur.data[index] = static_cast<uchar>(sum);//filter를 거친 값들을 uchar로 변환해서 src_gray_blur에 넣어준다.
		}
	}


	//copy col+1 to col, col[1]->col[0], left
	for (size_t row = 0; row < src_gray_blur.rows; row++)//y값0~511
	{
		//왼쪽 테두리 제거 
		int index_0 = row * src_gray_blur.cols + 0;//row*512 + 0
		int index_1 = row * src_gray_blur.cols + 1;//row*512 + 1
		src_gray_blur.data[index_0] = src_gray_blur.data[index_1];// 왼쪽 (0,row)<-(1,row)
		//오른쪽 테두리 제거
		index_0 = row * src_gray_blur.cols + (src_gray_blur.cols - 1);//row*512 + 511
		index_1 = row * src_gray_blur.cols + (src_gray_blur.cols - 2);//row*512 + 510
		src_gray_blur.data[index_0] = src_gray_blur.data[index_1];// 오른쪽 (511,row) <- (510,row)
	}
	//copy col to col+1, col[98]->col[99], right
	for (size_t col = 0; col < src_gray_blur.cols; col++)//x값 0~511
	{
		//윗 테두리 제거
		int index_0 = (0) * src_gray_blur.cols + col;//col
		int index_1 = (1) * src_gray_blur.cols + col;//col+512
		src_gray_blur.data[index_0] = src_gray_blur.data[index_1];//위 (col,0)<-(col,1)

		//아랫 테두리 제거
		index_0 = (src_gray_blur.rows - 1) * src_gray_blur.cols + col;//최하단 픽셀+col
		index_1 = (src_gray_blur.rows - 2) * src_gray_blur.cols + col;//최하단 바로 윗 픽셀+col
		src_gray_blur.data[index_0] = src_gray_blur.data[index_1];//아래 (col,511)<-(col,510)
	}

	return 1;
}