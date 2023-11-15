#include "ISP.h"

ISP::ISP()
{
	std::cout << "ISP::Ctor" << std::endl;
}

ISP::~ISP()
{
	std::cout << "ISP::Dtor" << std::endl;
}

bool ISP::Convert_BGR2GRAY(uchar* pBGR, int cols, int rows, uchar* pGray)
{
	if (pBGR == nullptr || pGray == nullptr)
	{
		return false;
	}

	//�̷��� ¥�� �Ʒ��� ���� performance�� �ſ� ����
	Mat src = Mat(rows, cols, CV_8UC3, pBGR);//color�ϱ� 3ä��
	Mat gray_cv = Mat(rows, cols, CV_8UC1, pGray);//color�ϱ� 3ä��
	cvtColor(src, gray_cv, COLOR_BGR2GRAY);
	return true;

	/*
	for (size_t row = 0; row < rows; row++)
	{
		for (size_t col = 0; col < cols; col++)
		{
			int index = (row)*cols + (col);
			int val_Y =
				0.299 * pBGR[index * 3 + 2]//r
				+ 0.587 * pBGR[index * 3 + 1]//g
				+ 0.114 * pBGR[index * 3 + 0];//b
			pGray[index] = (uchar)(val_Y + 0.5);//�ø��� �ϰ� ����ȯ���༭ ���� �ּ�ȭ

		}
	}
	return true;
	*/
}


bool ISP::Get_Histogram(uchar* pGray, int cols, int rows,int*pHisto, int histoSz)
{
	if (pGray == nullptr || pHisto == nullptr)//pGray�� pHisto�� ������ ����Ű�� ���� �ʴٸ�
	{
		return false;// false
	}


	
	for (size_t i = 0; i < cols*rows; i++)
	{
		if (pGray[i] >= histoSz)continue;//size 256�� �Ѵ´ٸ� �н� histogram�� ������ �ʰ� ���� �ɾ���
		pHisto[pGray[i]]++;//�ȳѴ´ٸ� histo �� ����
	}


	return true;
}


//���� ���� ��ο� ���� contrast�� �������� 180�̶�� enhancement �Ŀ��� 200������ �þ
bool ISP::Enhance_HistogramEq(uchar* pGray,int cols,int rows,uchar* pGrayEq)
{
	if (pGray == nullptr || pGrayEq == nullptr)//pGray�� pGrayEq�� ������ ����Ű�� ���� �ʴٸ�
	{
		return false;// false
	}
	int length = rows * cols;
	const int histoSz = 256;
	int histo[histoSz] = { 0, };

	Get_Histogram(pGray, cols, rows, histo, 256);

	//Ȯ�� �е� �Լ�
	int acc[histoSz] = { 0, };
	//acc[0]=histo[0]+histo[i] �� ���α׷����� �����ؾ���
	acc[0] = histo[0];
	for (size_t i = 1; i < histoSz; i++)
	{
		acc[i] = acc[i - 1] + histo[i];//���� Ȯ���е��Լ� �����ٰ� histo�� ������.
	}

	//new look up table ����... ������ �� ȭ�� ������ ���ο� ���������� ����
	int new_Gray[histoSz] = { 0, };
	for (size_t i = 1; i < histoSz; i++)
	{
		new_Gray[i] = static_cast<int>(1.0 * acc[i] * 255 / length);//���� Ȯ���е��Լ� �����ٰ� histo�� ������.
	}


	for (size_t i = 1; i < length; i++)
	{
		pGrayEq[i] = new_Gray[pGray[i]]; //���Ϸ�� ���� Equalization ������׷��� ���ν�����
	}





	//=======csv excel ���� ����
	std::ofstream ofile("histogram_eq.csv");

	std::string str = "brightness, histo, histo_eq";//���� ������ index ����
	ofile << str << "\n";//���� string �̶� ���� ���Ͽ� �־���
	if (ofile.is_open()) {
		for (size_t i = 0; i < histoSz; i++)
		{
			str = std::format("{}, {}, {}\n", i, histo[i], pGrayEq[i]);
			ofile << str;
		}
		ofile.close();
	}
	//=======csv excel ���� ����

	return true;


}