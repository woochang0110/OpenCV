#pragma once

#include <iostream>
#include <string>
#include <vector>

class GuGuDan
{
public:
	GuGuDan();
	~GuGuDan();
	//input
	void SetParams(size_t DanS = 0, size_t DanE = 0, size_t StepS = 0, size_t StepE = 0);
	//func
	int Proc();
	//output
	void Display();
private:
	size_t _DanS, _DanE, _StepS, _StepE;
	std::vector<std::string> vMsg;
};
