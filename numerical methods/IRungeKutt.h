#pragma once
#include <functional>

class IRungeKutt
{
public:
	virtual double getAnswerInStep(double x, double y, double h, const std::function<double(double, double)>& f) const = 0;
	// возвращает порядок метода
	virtual int getNumber() const = 0;
};