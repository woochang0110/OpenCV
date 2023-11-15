#include "GuGuDan.h"

GuGuDan::GuGuDan()
	: _DanS(0)
	, _DanE(0)
	, _StepS(0)
	, _StepE(0)
{
	std::cout << "GuGuDan::Ctor" << std::endl;
}

GuGuDan::~GuGuDan()
{
	std::cout << "GuGuDan::Dtor" << std::endl;
}

void GuGuDan::SetParams(size_t DanS, size_t DanE, size_t StepS, size_t StepE)
{
	std::cout << "GuGuDan::SetParams" << std::endl;

	this->_DanS = DanS;
	this->_DanE = DanE;
	this->_StepS = StepS;
	this->_StepE = StepE;

	_DanS = DanS;
	_DanE = DanE;
	_StepS = StepS;
	_StepE = StepE;

}

int GuGuDan::Proc()
{
	std::cout << "GuGuDan::Proc" << std::endl;

	vMsg.clear();//
	for (size_t step = _StepS; step <= _StepE; step++)
	{
		std::string msg = "";
		for (size_t dan = _DanS; dan <= _DanE; dan++)
		{
			size_t result = dan * step;
			std::string str = "";
			str = std::to_string(dan);
			str += "*";
			str += std::to_string(step);
			str += "=";
			str += std::to_string(result);
			msg += str + "\t";
			vMsg.push_back(msg);
		}
		return 1;
		//std::cout << msg << std::endl;
	}

	return 0;
}

void GuGuDan::Display()
{
	std::cout << "GuGuDan::Display" << std::endl;
	//for basic
	//for inRange(start, end, inc)

	for (size_t i = 0; i < vMsg.size(); i++)
	{
		std::cout << vMsg[i] << std::endl;
	}

	//for range with iter
	//int* pA = nullptr;
	//pA++;
	for (auto iter = vMsg.begin(); iter!=vMsg.end(); iter++)
	{
		std::cout << (*iter) << std::endl;
	}

	//for auto range
	for (const auto& msg : vMsg)
	{
		std::cout << msg << std::endl;
	}
}
