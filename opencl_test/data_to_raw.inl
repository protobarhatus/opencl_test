#ifndef DATA_TO_RAW_INL
#define DATA_TO_RAW_INL

#include "data_to_raw.h"
template<typename T>
std::vector<T> readClBufferAsVector(cl_mem buffer, cl_command_queue queue, int num_of_elements, int offset)
{
	std::vector<T> result(num_of_elements);

	clEnqueueReadBuffer(queue, buffer, true, offset * sizeof(T), num_of_elements * sizeof(T), result.data(), 0, NULL, NULL);

	return result;
}

template<typename T>
std::vector<std::vector<T>> readClBufferAsMatrix(cl_mem buffer, cl_command_queue queue, int lines, int columns)
{
	std::vector<std::vector<T>> result(lines);

	for (int i = 0; i < lines; ++i)
		result[i] = readClBufferAsVector<T>(buffer, queue, columns, columns * i);

	return result;
}
template<typename T>
cl_mem vectorToClBuffer(cl_context context, cl_command_queue queue, const std::vector<T>& data, bool copy_data,
	bool can_write, cl_mem buff , int offset)
{

	if (buff == NULL)
	{
		cl_int err;
		buff = clCreateBuffer(context, (can_write ? CL_MEM_READ_WRITE : CL_MEM_READ_ONLY), data.size() * sizeof(T), NULL, &err);
		if (!isOk(err))
			throw stringError(err);
	}


	clEnqueueWriteBuffer(queue, buff, copy_data, offset * sizeof(T), data.size() * sizeof(T), data.data(), 0, NULL, NULL);

	return buff;

}
template<typename T>
cl_mem vectorToClBuffer(cl_context context, cl_command_queue queue, const std::vector<std::vector<T>>& data, bool copy_data,
	bool can_write, cl_mem buff, int offset)
{
	if (buff == NULL)
	{
		cl_int err;
		buff = clCreateBuffer(context, (can_write ? CL_MEM_READ_WRITE : CL_MEM_READ_ONLY), data.size() * data[0].size() * sizeof(T), NULL, &err);
		if (!isOk(err))
			throw stringError(err);
	}
	for (int i = 0; i < data.size(); ++i)
		vectorToClBuffer<T>(context, queue, data[i], copy_data, can_write, buff, offset + i * data[0].size());
	return buff;
}


template<typename T>
void setKernelArg(cl_kernel kernel, int argnum, T arg_value)
{
	int err = clSetKernelArg(kernel, argnum, sizeof(T), &arg_value);
	if (!isOk(err))
		throw stringError(err);
}

#endif