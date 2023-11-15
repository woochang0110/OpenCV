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

	//이렇게 짜면 아래에 비해 performance가 매우 향상됨
	Mat src = Mat(rows, cols, CV_8UC3, pBGR);//color니까 3채널
	Mat gray_cv = Mat(rows, cols, CV_8UC1, pGray);//color니까 3채널
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
			pGray[index] = (uchar)(val_Y + 0.5);//올림을 하고 형변환해줘서 오차 최소화

		}
	}
	return true;
	*/
}


bool ISP::Get_Histogram(uchar* pGray, int cols, int rows,int*pHisto, int histoSz)
{
	if (pGray == nullptr || pHisto == nullptr)//pGray나 pHisto가 뭔가를 가리키고 있지 않다면
	{
		return false;// false
	}


	
	for (size_t i = 0; i < cols*rows; i++)
	{
		if (pGray[i] >= histoSz)continue;//size 256을 넘는다면 패스 histogram이 터지지 않게 조건 걸어줌
		pHisto[pGray[i]]++;//안넘는다면 histo 값 증가
	}


	return true;
}


//밝은 곳과 어두운 곳의 contrast가 기존에는 180이라면 enhancement 후에는 200정도로 늘어남
bool ISP::Enhance_HistogramEq(uchar* pGray,int cols,int rows,uchar* pGrayEq)
{
	if (pGray == nullptr || pGrayEq == nullptr)//pGray나 pGrayEq가 뭔가를 가리키고 있지 않다면
	{
		return false;// false
	}
	int length = rows * cols;
	const int histoSz = 256;
	int histo[histoSz] = { 0, };

	Get_Histogram(pGray, cols, rows, histo, 256);

	//확률 밀도 함수
	int acc[histoSz] = { 0, };
	//acc[0]=histo[0]+histo[i] 를 프로그램으로 구현해야함
	acc[0] = histo[0];
	for (size_t i = 1; i < histoSz; i++)
	{
		acc[i] = acc[i - 1] + histo[i];//이전 확률밀도함수 값에다가 histo값 더해줌.
	}

	//new look up table 생성... 영상의 각 화소 값들을 새로운 대응값으로 맵핑
	int new_Gray[histoSz] = { 0, };
	for (size_t i = 1; i < histoSz; i++)
	{
		new_Gray[i] = static_cast<int>(1.0 * acc[i] * 255 / length);//이전 확률밀도함수 값에다가 histo값 더해줌.
	}


	for (size_t i = 1; i < length; i++)
	{
		pGrayEq[i] = new_Gray[pGray[i]]; //계산완료된 값들 Equalization 히스토그램에 맵핑시켜줌
	}





	//=======csv excel 파일 생성
	std::ofstream ofile("histogram_eq.csv");

	std::string str = "brightness, histo, histo_eq";//제일 윗열에 index 세팅
	ofile << str << "\n";//위에 string 이랑 개행 파일에 넣어줌
	if (ofile.is_open()) {
		for (size_t i = 0; i < histoSz; i++)
		{
			str = std::format("{}, {}, {}\n", i, histo[i], pGrayEq[i]);
			ofile << str;
		}
		ofile.close();
	}
	//=======csv excel 파일 생성

	return true;


}