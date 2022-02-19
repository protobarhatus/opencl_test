#include <CL/opencl.h>
#include <vector>
#include <iostream>
#include <assert.h>
#include <random>

std::string stringError(cl_int err)
{
	switch (err)
	{
	case CL_INVALID_VALUE:
		return "Invalid value\n";
	case CL_INVALID_PLATFORM:
		return "Invalid platform\n";
	case CL_INVALID_DEVICE:
		return "Invalid device\n";
	case CL_INVALID_DEVICE_TYPE:
		return "Invalid device type\n";
	case CL_DEVICE_NOT_AVAILABLE:
		return "device in devices is currently not available even though the device was returned by clGetDeviceIDs\n";
	case CL_OUT_OF_HOST_MEMORY:
		return "there is a failure to allocate resources required by the OpenCL implementation on the host\n";
	case CL_INVALID_CONTEXT:
		return "invalid context\n";
	case CL_INVALID_QUEUE_PROPERTIES:
		return "values specified in properties are valid but are not supported by the device\n";
	case CL_INVALID_HOST_PTR:
		return "host_ptr is NULL and CL_MEM_USE_HOST_PTR or CL_MEM_COPY_HOST_PTR are set in flags or if host_ptr is not NULL but CL_MEM_COPY_HOST_PTR or CL_MEM_USE_HOST_PTR are not set in flags\n";
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		return "failure to allocate memory for buffer object\n";
	case CL_OUT_OF_RESOURCES:
		return "failure to allocate resources required by the OpenCL implementation on the device\n";
	case CL_INVALID_PROGRAM_EXECUTABLE:
		return "there is no successfully built program executable available for device associated with command_queue\n";
	case CL_INVALID_COMMAND_QUEUE:
		return "command_queue is not a valid command-queue\n";
	case CL_INVALID_KERNEL:
		return "kernel is not a valid kernel object\n";
	case CL_INVALID_KERNEL_ARGS:
		return "kernel argument values have not been specified\n";
	case CL_INVALID_WORK_DIMENSION:
		return "work_dim is not a valid value (i.e. a value between 1 and 3)\n";
	case CL_INVALID_WORK_GROUP_SIZE:
		return "local_work_size is specified and number of work-items specified by global_work_size is not evenly divisable by size of work-group given by local_work_size or does not match the work-group size specified for kernel using the __attribute__((reqd_work_group_size(X, Y, Z))) qualifier in program source\n\
OR local_work_size is specified and the total number of work-items in the work-group computed as local_work_size[0] *... local_work_size[work_dim - 1] is greater than the value specified by CL_DEVICE_MAX_WORK_GROUP_SIZE in the table of OpenCL Device Queries for clGetDeviceInfo\n\
OR local_work_size is NULL and the __attribute__((reqd_work_group_size(X, Y, Z))) qualifier is used to declare the work-group size for kernel in the program source\n";
	case CL_INVALID_WORK_ITEM_SIZE:
		return "the number of work-items specified in any of local_work_size[0], ... local_work_size[work_dim - 1] is greater than the corresponding values specified by CL_DEVICE_MAX_WORK_ITEM_SIZES[0], .... CL_DEVICE_MAX_WORK_ITEM_SIZES[work_dim - 1]\n";
	case CL_INVALID_GLOBAL_OFFSET:
		return "global_work_offset is not NULL\n";
	case CL_INVALID_EVENT_WAIT_LIST:
		return "event_wait_list is NULL and num_events_in_wait_list > 0, or event_wait_list is not NULL and num_events_in_wait_list is 0, or if event objects in event_wait_list are not valid events\n";
	default:
		return "uknown yet error\n";
	}
}

std::string clPlatformIdsError(cl_int err)
{
	switch (err)
	{
	case CL_INVALID_VALUE:
		return "error while processing clPlatformErrors\n";
	default:
		assert(false);
	}
}
std::string clPlatformInfoError(cl_int err)
{
	switch (err)
	{
	case CL_INVALID_PLATFORM:
		return "platform is not valid\n";
	case CL_INVALID_VALUE:
		return "param_name is not one of the supported values or size in bytes specified by param_value_size is less than size of return type";
	default:
		assert(false);
	}
}
std::string clDeviceIdsError(cl_int err)
{
	switch (err)
	{
	case CL_INVALID_PLATFORM:
		return "platform is not valid\n";
	case CL_INVALID_DEVICE_TYPE:
		return "device type is not valid\n";
	case CL_INVALID_VALUE:
		return "trying to ask devices list with zero buffer size\n";
	default:
		assert(false);
	}
}
std::string clDeviceInfoError(cl_int err)
{
	switch (err)
	{
	case CL_INVALID_DEVICE:
		return "invalid device in device info request\n";
	case CL_INVALID_VALUE:
		return "param_name is wrong or buffer is less then needed\n";
	default:
		assert(false);
	}
}

bool isOk(cl_int err)
{
	return err == CL_SUCCESS || err == CL_DEVICE_NOT_FOUND;
}

std::vector<cl_platform_id> getPlatforms()
{
	unsigned int num;
	clGetPlatformIDs(0, nullptr, &num);
	std::vector<cl_platform_id> res(num);

	auto err = clGetPlatformIDs(num, res.data(), NULL);
	if (!isOk(err))
		throw clPlatformIdsError(err);

	return res;
}
//enums for safety as harder write wrong value
enum DeviceType
{
	DEVICE_TYPE_CPU = CL_DEVICE_TYPE_CPU,
	DEVICE_TYPE_GPU = CL_DEVICE_TYPE_GPU,
	DEVICE_TYPE_ACCELERATOR = CL_DEVICE_TYPE_ACCELERATOR,
	DEVICE_TYPE_DEFAULT = CL_DEVICE_TYPE_DEFAULT,
	DEVICE_TYPE_ALL = CL_DEVICE_TYPE_ALL
};

std::vector<cl_device_id> getDevices(cl_platform_id platform, DeviceType type)
{
	unsigned int num;
	auto err = clGetDeviceIDs(platform, type, 0, NULL, &num);
	if (!isOk(err))
		throw clDeviceIdsError(err);

	if (num == 0 || err == CL_DEVICE_NOT_FOUND)
		return {};

	std::vector<cl_device_id> res(num);

	err = clGetDeviceIDs(platform, type, num, res.data(), NULL);
	if (!isOk(err))
		throw clDeviceIdsError(err);

	return res;
}

enum PlatformInfoType
{
	PLATFORM_PROFILE = CL_PLATFORM_PROFILE,
	PLATFORM_VERSION = CL_PLATFORM_VERSION,
	PLATFORM_NAME = CL_PLATFORM_NAME,
	PLATFORM_VENDOR = CL_PLATFORM_VENDOR,
	PLATFORM_EXTENSIONS = CL_PLATFORM_EXTENSIONS
};

std::string getInfo(cl_platform_id platform, PlatformInfoType type)
{
	size_t num;
	auto err = clGetPlatformInfo(platform, type, 0, NULL, &num);
	if (!isOk(err))
		throw clPlatformInfoError(err);

	char* buff = new char[num + 1];
	err = clGetPlatformInfo(platform, type, num, buff, NULL);
	if (!isOk(err))
		throw clPlatformInfoError(err);
	buff[num] = '\0';
	std::string res = buff;
	free(buff);
	return res;
}
//too many variants to create enum so use raw
std::string getInfo(cl_device_id device, cl_device_info type)
{
	size_t num;
	auto err = clGetDeviceInfo(device, type, 0, NULL, &num);
	if (!isOk(err))
		throw clDeviceInfoError(err);

	char* buff = new char[num + 1];
	err = clGetDeviceInfo(device, type, num, buff, NULL);
	if (!isOk(err))
		throw clDeviceInfoError(err);
	buff[num] = '\0';
	std::string res = buff;
	free(buff);
	return res;
}



void printPlatformsAndDevices()
{
	auto platforms = getPlatforms();

	for (auto& it : platforms)
	{
		std::cout << "Platform\n" << getInfo(it, PLATFORM_NAME) << "\n\nDevices:\n";
		auto devices = getDevices(it, DEVICE_TYPE_ALL);
		for (auto& it1 : devices)
			std::cout << getInfo(it1, CL_DEVICE_NAME) << "\n\n";
		std::cout << "\n\n\n";
	}
}


std::vector<cl_platform_id> getGpuPlatforms()
{
	auto platforms = getPlatforms();

	std::vector<cl_platform_id> res;

	for (auto& it : platforms)
	{
		auto gpus = getDevices(it, DEVICE_TYPE_GPU);
		if (gpus.size() > 0)
			res.push_back(it);
	}

	return res;
}

void cl_notify(const char* err_info, const void* private_info, size_t cb, void* user_data)
{
	throw "Error at context: " + std::string(err_info);
}

cl_context createContext(cl_platform_id platform)
{
	cl_context_properties properties[] = {
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0
	};

	auto devices = getDevices(platform, DEVICE_TYPE_GPU);
	cl_int err;
	cl_context res = clCreateContext(properties, devices.size(), devices.data(), &cl_notify, NULL, &err);

	if (!isOk(err))
		throw stringError(err);

	return res;

}

cl_command_queue createCommandQueue(cl_context context, cl_device_id devices)
{

	cl_int err;
#ifdef CL_TARGET_OPENCL_VERSION < 200
	auto res = clCreateCommandQueue(context, devices, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err);
#elif
	auto res = clCreateCommandQueueWithProperties(context, devices, NULL, &err);
#endif
	if (!isOk(err))
		throw stringError(err);

	return res;
}




template<typename T>
cl_mem vectorToClBuffer(cl_context context, cl_command_queue queue, const std::vector<T>& data, bool copy_data,
	bool can_write, cl_mem buff = NULL, int offset = 0)
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
	bool can_write = false, cl_mem buff = NULL, int offset = 0)
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




cl_mem createOutputBuffer(cl_context context, int size_in_bytes, bool readable = false)
{
	cl_int err;
	cl_mem buff = clCreateBuffer(context, readable ? CL_MEM_READ_WRITE : CL_MEM_WRITE_ONLY, size_in_bytes, NULL, &err);
	if (!isOk(err))
		throw stringError(err);
	return buff;
}
template<typename T>
std::vector<T> readClBufferAsVector(cl_mem buffer, cl_command_queue queue, int num_of_elements, int offset = 0)
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



#define STRINGIFY(...) #__VA_ARGS__


std::vector<std::vector<int>> randMatrix(int width, int height)
{
	std::vector<std::vector<int>> vec(width, std::vector<int>(height));
	for (auto& it : vec)
	{
		for (auto& it1 : it)
			it1 = rand() % 100 - 50;
	}
	return vec;
}



std::vector<std::vector<int>> operator+(const std::vector<std::vector<int>>& a, const std::vector<std::vector<int>>& b)
{
	assert(a.size() == b.size());
	assert(a[0].size() == b[0].size());
	std::vector<std::vector<int>> res(a.size(), std::vector<int>(a[0].size()));
	for (int i = 0; i < a.size(); ++i)
	{
		for (int j = 0; j < a[0].size(); ++j)
			res[i][j] = a[i][j] + b[i][j];
	}
	return res;
}
std::vector<std::vector<int>> operator*(const std::vector<std::vector<int>>& a, const std::vector<std::vector<int>>& b)
{
	assert(a[0].size() == b.size());
	std::vector<std::vector<int>> res(a.size(), std::vector<int>(b[0].size()));
	
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
std::vector<std::vector<int>> operator*(int num, const std::vector<std::vector<int>>& a)
{
	auto res = a;
	for (auto& it : res)
		for (auto& it1 : it)
			it1 *= num;
	return res;
}

std::ostream& operator<<(std::ostream& ost, const std::vector<int>& a)
{
	for (int i = 0; i < a.size(); ++i)
		ost << a[i] << ' ';
	return ost;
}
std::ostream& operator<<(std::ostream& ost, const std::vector<std::vector<int>>& a)
{
	for (int i = 0; i < a.size(); ++i)
		ost << a[i] << '\n';
	return ost;
}

template<typename T>
void setKernelArg(cl_kernel kernel, int argnum, T arg_value)
{
	int err = clSetKernelArg(kernel, argnum, sizeof(T), &arg_value);
	if (!isOk(err))
		throw stringError(err);
}



//i know i have lot of throwing extencions but i dont set try-catch because its actually more convenient when debugger
//throws you into place where throwing happened
int main()
{

	auto platforms = getGpuPlatforms();
	if (platforms.size() == 0)
	{
		std::cout << "no platforms with gpus available\n";
		return 0;
	}
	cl_context context = createContext(platforms[0]);

	cl_command_queue queue = createCommandQueue(context, *getDevices(platforms[0], DEVICE_TYPE_GPU).data());

	int X = 1000, AY = 1000, Y = 1000;

	auto A = randMatrix(X, AY), B = randMatrix(AY, Y), C = randMatrix(X, Y);

	int alpha = 2, beta = 3;
	
	cl_mem Abuff = vectorToClBuffer<int>(context, queue, A, true);
	cl_mem Bbuff = vectorToClBuffer<int>(context, queue, B, true);
	
	//cl_mem Cbuff = createOutputBuffer(context, A.size() * B[0].size() * sizeof(int), true);
	cl_mem Cbuff = vectorToClBuffer<int>(context, queue, C, true, true);
	

	const char* code = STRINGIFY(__kernel void gemm(__global int* A, __global int* B, __global int* C, int AX, int BX, int BY,
		int alpha, int beta)
	{
		int i = get_global_id(0);
		int j = get_global_id(1);
		
		int sum = 0;
		for (int k = 0; k < BX; ++k)
			sum += A[i * BX + k] * B[k * BY + j];

		C[i * BY + j] = alpha * sum + beta * C[i * BY + j];
	});


	cl_program program = clCreateProgramWithSource(context, 1, &code, NULL, NULL);
	int ret = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

	
	cl_kernel kernel = clCreateKernel(program, "gemm", NULL);

	setKernelArg(kernel, 0, Abuff);
	setKernelArg(kernel, 1, Bbuff);
	setKernelArg(kernel, 2, Cbuff);

	setKernelArg(kernel, 3, A.size());
	setKernelArg(kernel, 4, A[0].size());
	setKernelArg(kernel, 5, B[0].size());
	setKernelArg(kernel, 6, alpha);
	setKernelArg(kernel, 7, beta);

	size_t work_size[2] = { A.size(), B[0].size() };

	time_t t1, t2;
	t1 = clock();
	int res = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, work_size, NULL, 0, NULL, NULL);
	
	if (!isOk(res))
		throw stringError(res);


	auto Creaden = readClBufferAsMatrix<int>(Cbuff, queue, A.size(), B[0].size());
	t2 = clock();
	std::cout << "On GPU: " << t2 - t1 << "\n";

	t1 = clock();
	auto check = alpha * A * B + beta * C;
	t2 = clock();
	std::cout << "On CPU: " << t2 - t1 << "\n";

	if (Creaden == check)
		std::cout << "Correct!\n";
	else
	{
		std::cout << A << "\n\n" << B << "\n\n";
		std::cout << C << "\n\n" << 2*A*B + 3*C << "\n\n" << Creaden;
		//std::cout << "B from Buff" << readClBufferAsMatrix<int>(Bbuff, queue, B.size(), B[0].size());
	}

	clReleaseMemObject(Abuff);
	clReleaseMemObject(Bbuff);
	clReleaseMemObject(Cbuff);

}