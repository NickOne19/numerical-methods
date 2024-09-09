#include "Solution.h"
#include "RungeKutt113.h"
#include "RungeKutt112.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <vector>

std::vector<std::string> getSequence(const std::string& str, const std::string& delimiter)
{
	std::vector<std::string> answ;
	size_t posNew = std::string::npos;
	int posLast = -1 * delimiter.size();
	std::string token;

	while ((posNew = str.find(delimiter, posLast + delimiter.size())) != std::string::npos) {
		token = str.substr(posLast + delimiter.size(), posNew - posLast - delimiter.size());
		if (!token.empty())
		{
			answ.push_back(token);
		}
		posLast = posNew;
		token.erase();
	}

	token = str.substr(posLast + delimiter.size());
	if (!token.empty())
	{
		answ.push_back(token);
	}

	return answ;
}

double f(double x, double y)
{
	return x - y / 10;
}

int main()
{
	double A = 0.0;
	double B = 0.0;
	double C = A;
	double y0 = 0.0;
	double hmin = 0.0;
	double eps = 0.0;

	// получение данных

	std::ifstream file("data.txt");

	std::string str;
	std::getline(file, str);
	std::vector<std::string> tmp{ getSequence(str, " ") };
	A = std::stod(tmp[0]);
	B = std::stod(tmp[1]);
	C = std::stod(tmp[2]);
	y0 = std::stod(tmp[3]);

	std::getline(file, str);
	tmp = getSequence(str, " ");
	hmin = std::stod(tmp[0]);
	eps = std::stod(tmp[1]);
	file.close();
	
	// решение

	Solution task(A, B, C, y0, f);
	task.findAnswer(hmin, eps, RungeKutt113(), RungeKutt112());
}