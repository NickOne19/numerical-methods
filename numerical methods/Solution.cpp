#include "Solution.h"
#include <fstream>
#include <iostream>

namespace
{
	// проверяет входные данные
	Error isGoodCond(double A, double B, double C)
	{
		if (C != A && C != B)
			return Error::ERROR2;
		if (A >= B)
			return Error::ERROR2;
		return Error::ERROR0;
	}

	// находит локальную погрешность и записывает её в файл
	double findEps(std::ofstream& file, double y, double yApproximate)
	{
		file << yApproximate - y << "\n";
		return yApproximate - y;
	}
}

Solution::Solution(double A, double B, double C, double y0, const std::function<double(double, double)>& f) : f(f) 
{
	this->A = A;
	this->B = B;
	this->C = C;
	this->y0 = y0;

	numberOfBadX = 0;
	numberOfGoodX = 0;
	numberOfminh = 0;
}

Error Solution::findAnswer(double hmin, double eps, const IRungeKutt& met1, const IRungeKutt& metApproximate)
{
	// открытие файла для записи
	std::ofstream file("rez.txt");

	ValuesXYH xyh{ C, y0, (B - A) / 10 };
	Error answ = isGoodCond(A, B, C);

	if (answ != Error::ERROR0)
		return answ;
	else
	{
		Direction dir = this->findDirection();
		bool isEnd = false;

		while (!isEnd)
		{
			xyh = this->iteration(file, dir, xyh, hmin, eps, met1, metApproximate);

			if (this->endMethodCompare(xyh.x, dir, xyh.h) < hmin)
				isEnd = true;

			numberOfGoodX++;
		}

		// окончание работы метода
		// ***отлажено только для случая Direction::toRight
		if (this->endMethodCompare(xyh.x, dir) >= 2 * hmin)
		{
			ValuesXYH xn_1{ B - hmin, met1.getAnswerInStep(xyh.x, xyh.y, B - hmin - xyh.x, f), hmin };
			file << xn_1.x << " " << xn_1.y << " " << xn_1.h << "\n";
			ValuesXYH xn{ B, met1.getAnswerInStep(xn_1.x, xn_1.y, xn_1.h, f), 0.0 };
			file << xn.x << " " << xn.y << " " << xn.h << "\n";

			// заполнение итоговой строки
			file << (numberOfGoodX += 2) << " ";

		}
		else if (this->endMethodCompare(xyh.x, dir) > 1.5 * hmin)
		{
			ValuesXYH xn_1{ xyh.x + (B - xyh.x) / 2, met1.getAnswerInStep(xyh.x, xyh.y, (B - xyh.x) / 2, f), hmin };
			file << xn_1.x << " " << xn_1.y << " " << xn_1.h << "\n";
			ValuesXYH xn{ B, met1.getAnswerInStep(xn_1.x, xn_1.y, xn_1.h, f), 0.0 };
			file << xn.x << " " << xn.y << " " << xn.h << "\n";

			// заполнение итоговой строки
			file << (numberOfGoodX += 2) << " ";
		}
		else
		{
			ValuesXYH xn{ B, met1.getAnswerInStep(xyh.x, xyh.y, B - xyh.x, f), 0.0 };
			file << xn.x << " " << xn.y << " " << xn.h << "\n";

			// заполнение итоговой строки
			file << ++numberOfGoodX << " ";
		}

		// заполнение итоговой строки
		file << numberOfBadX << " " << numberOfminh << "\n";
		return answ;
	}
}

Solution::ValuesXYH Solution::iteration(std::ofstream& file, Direction dir, const ValuesXYH& old, double hMin, double eps, const IRungeKutt& met1, const IRungeKutt& metApproximate)
{
	double hNew = old.h;
	while (hNew > hMin)
	{
		double xNew = 0.0;
		if (dir == Direction::toRight)
			xNew = old.x + hNew;
		else
			xNew = old.x - hNew;

		double yNew = met1.getAnswerInStep(xNew, old.y, hNew, f);
		file << xNew << " " << yNew << " " << hNew << " ";
		double yApproximate = metApproximate.getAnswerInStep(xNew, old.y, hNew, f);
		double apprEps = findEps(file, yNew, yApproximate);
		++numberOfBadX;

		if (std::abs(apprEps) > eps)
			hNew /= 2;
		else
		{
			--numberOfBadX;
			double k = pow(2, met1.getNumber());
			if (std::abs(apprEps) < eps / k)
				return Solution::ValuesXYH{ xNew, yNew, hNew * 2 };
			else
				return Solution::ValuesXYH{ xNew, yNew, hNew };
		}
	}

	// обработка случая достижения шагом минимально-допустимого значения
	++numberOfminh;
	return Solution::ValuesXYH{ old.x + hMin, met1.getAnswerInStep(old.x, old.y, hMin, f), hMin };
}

Solution::Direction Solution::findDirection() const
{
	if (C == A)
		return Solution::Direction::toRight;
	else
		return Solution::Direction::toLeft;
}

// h - костыль жёсткий, но так проще всего
double Solution::endMethodCompare(double x, Direction dir, double h) const
{
	if (dir == Direction::toRight)
		return B - (x + h);
	else
		return (x - h) - A;
}
