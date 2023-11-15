#include <iostream>
#include <vector>
#include <string>
class Shape
{
public:
	Shape()
		:a(0)//member initialize
		, b(10)
		, pA(nullptr)
	{
		//copy operator
		a = 1;
		b = 2;
		std::cout << "Ctor" << std::endl;
	}
	~Shape();
private:
	int a, b;
	int* pA;
};
//#define CIRCLE 100
//#define RECTANGE 101
const size_t _CIRCLE = 100;

enum EShape {
	eCircle=0,
	eRectangle,
	eTriangle,
	ePoly,
	eShapeMax
};

int main()
{
	int a = 1;
	//int ctor->instance->|a|->operator '=' -> 1
	int b(20);
	//int ctor->instance + 2 (initilized)
	int c = a + b;
	std::cout << c << "\t=\t" << a << "\t+\t" << b << std::endl;

	int* pA = nullptr;
	std::cout << "size of pointer pA = " << sizeof(pA) << std::endl;

	EShape _eShape = EShape::eCircle;
	//resource create
	int* pShape = nullptr;
	for (size_t i = 0; i < 100; i++)
	{
		pShape = new int[EShape::eShapeMax];//smart ptr
		//init and management
		memset((void*)pShape, 0, sizeof(int) * EShape::eShapeMax);
		//destory
		if (pShape != nullptr)
		{
			delete[] pShape;
			pShape = nullptr;
		}
	}

	std::vector<int> vShape(EShape::eShapeMax, 0);
	vShape[0] = 1;
	vShape[1] = 2;
	vShape[2] = 3;
	vShape[3] = 4;
	
	//Quize
	//if...for...
	// GuGuDan
	// input  : dan_start, end(2~9)
	// intput : step_start, end(1~9)
	
	//dan : 2~9, step : 1~9
	//2*1 = 2      3*1=3    ....     9*1=9
	//2*2 = 4      3*2=6    ....     9*2=18
	//
	//2*9 = 18     3*9=27   ....     9*9=81

	//dan : 2~9, step : 9~9
	//2*9 = 18     3*9=27   ....     9*9=81
	int DanS, DanE, StepS, StepE;
	DanS = 2, DanE = 5, StepS = 1, StepE = 9;
	for (size_t step = StepS; step <= StepE; step++)
	{
		std::string msg = "";
		for (size_t dan = DanS; dan <= DanE; dan++)
		{
			int result = dan * step;
			std::string str = "";
			str = std::to_string(dan);
			str += "*";
			str += std::to_string(step);
			str += "=";
			str += std::to_string(result);
			msg += str + "\t";
		}
		std::cout << msg << std::endl;
	}
	return 1;
}
