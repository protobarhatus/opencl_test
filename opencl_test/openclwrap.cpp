#include "openclwrap.h"
#include "error_treatment.h"
#include <vector>
#include <string>


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
		throw stringError(err);

	return res;
}


std::vector<cl_device_id> getDevices(cl_platform_id platform, DeviceType type)
{
	unsigned int num;
	auto err = clGetDeviceIDs(platform, type, 0, NULL, &num);
	if (!isOk(err))
		throw stringError(err);

	if (num == 0 || err == CL_DEVICE_NOT_FOUND)
		return {};

	std::vector<cl_device_id> res(num);

	err = clGetDeviceIDs(platform, type, num, res.data(), NULL);
	if (!isOk(err))
		throw stringError(err);

	return res;
}



std::string getInfo(cl_platform_id platform, PlatformInfoType type)
{
	size_t num;
	auto err = clGetPlatformInfo(platform, type, 0, NULL, &num);
	if (!isOk(err))
		throw stringError(err);

	char* buff = new char[num + 1];
	err = clGetPlatformInfo(platform, type, num, buff, NULL);
	if (!isOk(err))
		throw stringError(err);
	buff[num] = '\0';
	std::string res = buff;
	free(buff);
	return res;
}

int getIntInfo(cl_device_id device, cl_device_info type)
{

	size_t res;
	int err = clGetDeviceInfo(device, type, sizeof(size_t), &res, NULL);
	if (!isOk(err))
		throw stringError(err);

	return res;
}
//too many variants to create enum so use raw
std::string getInfo(cl_device_id device, cl_device_info type)
{
	size_t num;
	auto err = clGetDeviceInfo(device, type, 0, NULL, &num);
	if (!isOk(err))
		throw stringError(err);

	char* buff = new char[num + 1];
	err = clGetDeviceInfo(device, type, num, buff, NULL);
	if (!isOk(err))
		throw stringError(err);
	buff[num] = '\0';
	std::string res = buff;
	free(buff);
	return res;
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



cl_context createContext(cl_platform_id platform)
{
	cl_context_properties properties[] = {
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0
	};

	auto devices = getDevices(platform, DEVICE_TYPE_GPU);
	cl_int err;
	cl_context res = clCreateContext(properties, devices.size(), devices.data(), NULL, NULL, &err);

	if (!isOk(err))
		throw stringError(err);

	return res;

}

cl_command_queue createCommandQueue(cl_context context, cl_device_id devices)
{

	cl_int err;
#if (CL_TARGET_OPENCL_VERSION < 200)
	auto res = clCreateCommandQueue(context, devices, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err);
#else
	auto res = clCreateCommandQueueWithProperties(context, devices, NULL, &err);
#endif
	if (!isOk(err))
		throw stringError(err);

	return res;
}


cl_mem createOutputBuffer(cl_context context, int size_in_bytes, bool readable)
{
	cl_int err;
	cl_mem buff = clCreateBuffer(context, readable ? CL_MEM_READ_WRITE : CL_MEM_WRITE_ONLY, size_in_bytes, NULL, &err);
	if (!isOk(err))
		throw stringError(err);
	return buff;
}
