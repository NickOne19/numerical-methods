#pragma once
#include "IRungeKutt.h"

class RungeKutt112 : public IRungeKutt
{
public:
	double getAnswerInStep(double x, double y, double h, const std::function<double(double, double)>& f) const override
	{
		double k1 = h * f(x, y);
		double k2 = h * f(x + h / 2, y + k1 / 2);
		double k3 = h * f(x + h, y - k1 + 2 * k2);

		return y + (k1 + 4 * k2 + k3) / 6;
	}

	int getNumber() const override
	{
		return 3;
	}
};