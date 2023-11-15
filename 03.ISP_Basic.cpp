#pragma once
#include "Common.h"

int main()
{
	std::string fileName = "../KCCImageNet/stinkbug.png";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);//������� �̹���(src) �����ֱ� 1ch
	cv::resize(src, src, cv::Size(src.cols * 2, src.rows * 2));//�̹��� 2��� ��Ƣ�� cols����2��  rows����2��
	int datas[10] = { 5,3,1,4,6,4,8,4,3 };
	int* pDatas = datas;//data array

	uchar* pData = src.data;//data array...start[0,1,2,3,4,5]
	int length = src.total();//data length// �̹��� �ȼ� ���� �����ȼ�*�����ȼ� ��
	int channels = src.channels();//�̹��� ch ����
	//length *= channels;

	//HISTOGRAM: ���������� ��Ÿ���� �׷���

	const int histoSz = 256;//sizeof(uchar);
	int histo[histoSz] = { 0, };//HISTO ��� �󵵼� 
	//int* pHisto = new int[length];

	for (size_t i = 0; i < length; i++)
	{
		histo[pData[i]]++;
	}

	uchar* data = src.data;
	int size = src.rows * src.cols;//��(����)*��(����)
	auto result = std::minmax_element(pData, pData + length);
	double mean_brightness = cv::mean(src)[0];//mean�� �迭�� ��հ��� �����ִ� �Լ�
	auto _minmax = std::minmax_element(histo, histo + histoSz);//pair(int 2��) ��������  
	int _min_value = *_minmax.first;//pair(int 2�� �� ù��°) ��������  
	int _max_value = *_minmax.second;//pair(int 2���� �ι�°) ��������  

	//=========���� â�� ������׷� ǥ���ϱ�
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
	//=========���� â�� ������׷� ǥ���ϱ� ��


	//=========���Ӱ� �̹���(draw) �ϳ� ���� �̹��� ���� ������׷� ǥ���ϱ�
	cv::Mat draw = src.clone();//draw<- src �̹��� ����
	cvtColor(draw, draw, ColorConversionCodes::COLOR_GRAY2BGR);//draw �̹����� ���µ� color�� ��鿡�� BGR�� ��ȯ���ְ�
	for (size_t row = 0; row < histoSz; row++)//cols����  rows����
	{
		int scaled_cols = (histo[row] * 1.0 / _max_value) * src.cols;
		cv::Point st = Point(0, row * 2.7);//�׳� row ������ �ϴϱ� ������׷��� ���ʿ����� ��µǰ� ������ �̹����� �°� ��� �� ������ ����� ��µǵ��� 2.7�� �����ִ°���
		cv::Point ed = Point(scaled_cols, row * 2.7);//�׳� row ������ �ϴϱ� ������׷��� ���ʿ����� ��µǰ� ������ �̹����� �°� ��� �� ������ ����� ��µǵ��� 2.7�� �����ִ°���
		cv::line(draw, st, ed, CV_RGB(255, 255, 0));
	}
	//=========���Ӱ� �̹���(draw) �ϳ� ���� �̹��� ���� ������׷� ǥ���ϱ�

	return 1;

}