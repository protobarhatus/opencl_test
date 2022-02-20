#ifndef USEFUL_VECTOR_FUNCS_H
#define USEFUL_VECTOR_FUNCS_H
#include <vector>

template <typename T>
std::vector<std::vector<T>> randMatrix(int width, int height);

template <typename T>
std::vector<std::vector<T>> operator+(const std::vector<std::vector<T>>& a, const std::vector<std::vector<T>>& b);

template <typename T>
std::vector<std::vector<T>> operator*(const std::vector<std::vector<T>>& a, const std::vector<std::vector<T>>& b);

template <typename T>
std::vector<std::vector<T>> operator*(T num, const std::vector<std::vector<T>>& a);

template <typename T>
std::ostream& operator<<(std::ostream& ost, const std::vector<T>& a);

template <typename T>
std::ostream& operator<<(std::ostream& ost, const std::vector<std::vector<T>>& a);

template <typename T>
bool operator==(const std::vector<std::vector<T>>& a, const std::vector<std::vector<T>>& b);

template <>
bool operator==<float>(const std::vector<std::vector<float>>& a, const std::vector<std::vector<float>>& b);

#include "useful_vector_funcs.inl"


#endif