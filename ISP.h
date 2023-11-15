#pragma once

#include "Common.h"

class ISP
{
public:
	ISP();
	~ISP();

	bool Convert_BGR2GRAY(
		uchar* pBGR,//color channel, input
		int cols,//cols, input
		int rows,//rows, input
		uchar* pGray//mono channel, output
	);

	bool Get_Histogram(
		uchar * pGray,
		int cols,
		int rows,
		int* pHisto,
		int histoSz
	);
	bool Enhance_HistogramEq(
		uchar* pGray,
		int cols,//cols, input
		int rows,//rows, input
		uchar* pGrayEq
	);
	
private:

};

