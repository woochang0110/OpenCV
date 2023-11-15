#pragma once
#include "Common.h"

int main()
{
	//..\thirdparty\opencv-4.8.0_wnd\sources\samples\data
	// "D:\woochangmin\C++\src\_AI\VisionAPP\thirdparty\opencv-4.8.0_wnd\sources\samples\data\lena.jpg"
	//../KCCImageNet/stinkbug.png
	std::string fileName = "../thirdparty/opencv-4.8.0_wnd/sources/samples/data/lena.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);//������� �̹���(src) �����ֱ� 1ch

	//RGB�� 3CH�̱� ������ ������ �����͸� ���� �迭�� ����� �����.
	uchar* pData = src.data;//data array...start[0,1,2,3,4,5]
	int length = src.total();//data length// �̹��� �ȼ� ���� �����ȼ�*�����ȼ� ��
	int channels = src.channels();//�̹��� ch ����


	cv::Mat src_Y = cv::Mat(src.rows, src.cols, CV_8UC1);//Y ���Ŀ� ���� RGB 3ch�� ���� �� Y�� �ش��ϴ� ������ 1ch�� ǥ��
	cv::Mat src_Cb = cv::Mat(src.rows, src.cols, CV_8UC1);//Cb ���Ŀ� ���� RGB 3ch�� ���� �� Cb�� �ش��ϴ� ������ 1ch�� ǥ��
	cv::Mat src_Cr = cv::Mat(src.rows, src.cols, CV_8UC1);//Cr ���Ŀ� ���� RGB 3ch�� ���� �� Cr�� �ش��ϴ� ������ 1ch�� ǥ��


	cv::Mat src_New = cv::Mat(src.rows, src.cols, CV_8UC3);

	cv::Mat src_Diff = cv::Mat(src.rows, src.cols, CV_8UC3);


	for (size_t row = 0; row < src.rows; row++)
	{
		for (size_t col = 0; col < src.cols; col++)
		{
			int index = (row)*src.cols + (col);//mono->�� �迭�� ���� �� index�� ����� �ִ°���.

			if (channels == 3)//RGB->YCbCr
			{
				int index_B = (row)*src.cols * 3 + (col * 3 + 0);//RGB->B �迭�� ���� �� index�� ����� �ִ°���.
				int index_G = (row)*src.cols * 3 + (col * 3 + 1);//RGB->G �迭�� ���� �� index�� ����� �ִ°���.
				int index_R = (row)*src.cols * 3 + (col * 3 + 2);//RGB->R �迭�� ���� �� index�� ����� �ִ°���.

				int val_B = pData[index_B];
				int val_G = pData[index_G];
				int val_R = pData[index_R];
				//RGB -> Y Cb Cr
				int val_Y = 0.299 * val_R + 0.587 * val_G + 0.114 * val_B;		//RGB ->Y ���Ŀ� ���� RGB 3ch�� ���� �� Y�� �ش��ϴ� ������ 1ch�� ǥ��
				int val_Cb = -0.169 * val_R - 0.331 * val_G + 0.500 * val_B;	//RGB ->Cb ���Ŀ� ���� RGB 3ch�� ���� �� Cb�� �ش��ϴ� ������ 1ch�� ǥ��
				int val_Cr = 0.500 * val_R - 0.419 * val_G - 0.0813 * val_B;	//RGB ->Cr ���Ŀ� ���� RGB 3ch�� ���� �� Cr�� �ش��ϴ� ������ 1ch�� ǥ��



				src_Y.data[index] = (uchar)val_Y;//uchar�� ����ȯ�ؼ� src_Y�̹����� ǥ��
				src_Cb.data[index] = (uchar)val_Cb;//uchar�� ����ȯ�ؼ� src_Cb�̹����� ǥ��
				src_Cr.data[index] = (uchar)val_Cr;//uchar�� ����ȯ�ؼ� src_Cr�̹����� ǥ��
				//Y Cb Cr      -> RGB 	
				int val_NR = 1.000 * val_Y + 1.402 * val_Cr + 0.000 * val_Cb;			//Y Cb Cr->R ���Ŀ� ���� RGB 3ch�� ���� �� Y�� �ش��ϴ� ������ 3ch �� �ϳ��� ch ���� �־���
				int val_NG = 1.000 * val_Y - 0.714 * val_Cr - 0.344 * val_Cb;			//Y Cb Cr->G ���Ŀ� ���� RGB 3ch�� ���� �� Cb�� �ش��ϴ� ������ 3ch �� �ϳ��� ch ���� �־���
				int val_NB = 1.000 * val_Y + 0.000 * val_Cr + 1.772 * val_Cb;			//Y Cb Cr->B ���Ŀ� ���� RGB 3ch�� ���� �� Cr�� �ش��ϴ� ������ 3ch �� �ϳ��� ch ���� �־���


				src_New.data[index_B] = (uchar)val_NB;
				src_New.data[index_G] = (uchar)val_NG;
				src_New.data[index_R] = (uchar)val_NR;

			}
		}
	}
	src_Diff = src - src_New;

	return 1;
}