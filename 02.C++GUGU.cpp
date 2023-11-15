#include <iostream>
#include <vector>
#include <string>
//#1. function of GUGUDAN
//#2. class of GUGUDAN
//    setParams, proc, display

#include "GuGuDan.h"


int main()
{
	int DanS, DanE, StepS, StepE;
	DanS = 2, DanE = 9, StepS = 1, StepE = 9;

	GuGuDan ggd;
	ggd.SetParams(DanS, DanE, StepS, StepE);
	ggd.Proc();
	ggd.Display();

	DanS = 5, DanE = 6, StepS = 2, StepE = 3;
	ggd.SetParams(DanS, DanE, StepS, StepE);
	ggd.Proc();
	ggd.Display();

	DanS = 7, DanE = 7, StepS = 2, StepE = 3;
	ggd.SetParams(DanS, DanE, StepS, StepE);
	ggd.Proc();
	ggd.Display();

	DanS = 3, DanE = 5, StepS = 7, StepE = 7;
	ggd.SetParams(DanS, DanE, StepS, StepE);
	ggd.Proc();
	ggd.Display();


	return 1;


	if (false)
	{
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
		DanS = 2, DanE = 9, StepS = 1, StepE = 9;
		for (size_t step = StepS; step <= StepE; step++)
		{
			std::string msg = "";
			for (size_t dan = DanS; dan <= DanE; dan++)
			{
				size_t result = dan * step;
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
	}

	return 1;
}
