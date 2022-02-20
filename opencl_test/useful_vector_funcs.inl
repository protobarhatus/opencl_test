#ifndef USEFUL_VECTOR_FUNCS_INL
#define USEFUL_VECTOR_FUNCS_INL

#include "useful_vector_funcs.h"
#include <vector>
#include <assert.h>
#include <iostream>

template <typename T>
std::vector<std::vector<T>> randMatrix(int width, int height)
{
	std::vector<std::vector<T>> vec(width, std::vector<T>(height));
	for (auto& it : vec)
	{
		for (auto& it1 : it)
			it1 = rand() % 100 - 50;
	}
	return vec;
}


template <typename T>
std::vector<std::vector<T>> operator+(const std::vector<std::vector<T>>& a, const std::vector<std::vector<T>>& b)
{
	assert(a.size() == b.size());
	assert(a[0].size() == b[0].size());
	std::vector<std::vector<T>> res(a.size(), std::vector<T>(a[0].size()));
	for (int i = 0; i < a.size(); ++i)
	{
		for (int j = 0; j < a[0].size(); ++j)
			res[i][j] = a[i][j] + b[i][j];
	}
	return res;
}

template <typename T>
std::vector<std::vector<T>> operator*(const std::vector<std::vector<T>>& a, const std::vector<std::vector<T>>& b)
{
	assert(a[0].size() == b.size());
	std::vector<std::vector<T>> res(a.size(), std::vector<T>(b[0].size()));

	for (int i = 0; i < a.size(); ++i)
	{
		for (int j = 0; j < b[0].size(); ++j)
		{
			for (int k = 0; k < a[0].size(); ++k)
				res[i][j] += a[i][k] * b[k][j];
		}
	}
	return res;
}

template <typename T>
std::vector<std::vector<T>> operator*(T num, const std::vector<std::vector<T>>& a)
{
	auto res = a;
	for (auto& it : res)
		for (auto& it1 : it)
			it1 *= num;
	return res;
}

template <typename T>
std::ostream& operator<<(std::ostream& ost, const std::vector<T>& a)
{
	for (int i = 0; i < a.size(); ++i)
		ost << a[i] << ' ';
	return ost;
}

template <typename T>
std::ostream& operator<<(std::ostream& ost, const std::vector<std::vector<T>>& a)
{
	for (int i = 0; i < a.size(); ++i)
		ost << a[i] << '\n';
	return ost;
}
template <typename T>
bool operator==(const std::vector<std::vector<T>>& a, const std::vector<std::vector<T>>& b)
{
	if (a.size() != b.size() || a[0].size() != b[0].size())
		return false;

	for (int i = 0; i < a.size(); ++i)
	{
		for (int j = 0; j < a[0].size(); ++j)
		{
			if (a[i][j] != b[i][j])
				return false;
		}
	}
	return true;
}
template <>
bool operator==<float>(const std::vector<std::vector<float>>& a, const std::vector<std::vector<float>>& b)
{
	if (a.size() != b.size() || a[0].size() != b[0].size())
		return false;

	for (int i = 0; i < a.size(); ++i)
	{
		for (int j = 0; j < a[0].size(); ++j)
		{
			if (std::abs(a[i][j] - b[i][j]) >= 0.0001)
				return false;
		}
	}
	return true;
}


#endif