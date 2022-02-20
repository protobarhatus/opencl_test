#ifndef DATA_TO_RAW_H
#define DATA_TO_RAW_H
#include <CL/opencl.h>
#include <vector>

template<typename T>
cl_mem vectorToClBuffer(cl_context context, cl_command_queue queue, const std::vector<T>& data, bool copy_data,
	bool can_write, cl_mem buff = NULL, int offset = 0);

template<typename T>
cl_mem vectorToClBuffer(cl_context context, cl_command_queue queue, const std::vector<std::vector<T>>& data, bool copy_data,
	bool can_write = false, cl_mem buff = NULL, int offset = 0);

template<typename T>
std::vector<T> readClBufferAsVector(cl_mem buffer, cl_command_queue queue, int num_of_elements, int offset = 0);

template<typename T>
std::vector<std::vector<T>> readClBufferAsMatrix(cl_mem buffer, cl_command_queue queue, int lines, int columns);

template<typename T>
void setKernelArg(cl_kernel kernel, int argnum, T arg_value);

#include "data_to_raw.inl"

#endif