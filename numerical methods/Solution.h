#pragma once
#include "IRungeKutt.h"
#include <functional>
#include <fstream>

enum class Error
{
	ERROR0,
	ERROR1,
	ERROR2
};

class Solution
{
	// enum для обозначения напрвления счёта выражения
	enum class Direction
	{
		toRight,
		toLeft
	};

	// доп структура
	struct ValuesXYH
	{
		double x;
		double y;
		double h;
	};

	double A;
	double B;
	double C;
	double y0;
	std::function<double(double, double)> f;

	int numberOfGoodX;
	int numberOfBadX;
	int numberOfminh;

public:

	Solution(double A, double B, double C, double y0, const std::function<double(double, double)>& f);
	Error findAnswer(double hmin, double eps, const IRungeKutt& met1, const IRungeKutt& metApproximate);

private:

	// находит направление решения в заданном условии
	Direction findDirection() const;

	ValuesXYH iteration(std::ofstream& file, Direction dir, const ValuesXYH& old, double hMin, double eps, const IRungeKutt& met1, const IRungeKutt& metAppr);
	
	// возвращает значение выражения для определения конца работы метода
	double endMethodCompare(double x, Direction dir, double h = 0.0) const;
};