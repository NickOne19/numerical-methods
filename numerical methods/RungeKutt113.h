#pragma once
#include "IRungeKutt.h"

class RungeKutt113 : public IRungeKutt
{
public:
	double getAnswerInStep(double x, double y, double h, const std::function<double(double, double)>& f) const override
	{
		double k1 = h * f(x, y);
		double k2 = h * f(x + h / 2, y + k1 / 2);

		return y + k2;
	}

	int getNumber() const override
	{
		return 2;
	}
};