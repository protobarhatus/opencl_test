#ifndef OPENCLWRAP_H
#define OPENCLWAP_H
#include <CL/opencl.h>
#include <string>
#include "data_to_raw.h"

//enums for safety as harder write wrong value
enum DeviceType
{
	DEVICE_TYPE_CPU = CL_DEVICE_TYPE_CPU,
	DEVICE_TYPE_GPU = CL_DEVICE_TYPE_GPU,
	DEVICE_TYPE_ACCELERATOR = CL_DEVICE_TYPE_ACCELERATOR,
	DEVICE_TYPE_DEFAULT = CL_DEVICE_TYPE_DEFAULT,
	DEVICE_TYPE_ALL = CL_DEVICE_TYPE_ALL
};
enum PlatformInfoType
{
	PLATFORM_PROFILE = CL_PLATFORM_PROFILE,
	PLATFORM_VERSION = CL_PLATFORM_VERSION,
	PLATFORM_NAME = CL_PLATFORM_NAME,
	PLATFORM_VENDOR = CL_PLATFORM_VENDOR,
	PLATFORM_EXTENSIONS = CL_PLATFORM_EXTENSIONS
};


bool isOk(cl_int err);
std::vector<cl_platform_id> getPlatforms();
std::vector<cl_device_id> getDevices(cl_platform_id platform, DeviceType type);

std::string getInfo(cl_platform_id platform, PlatformInfoType type);
int getIntInfo(cl_device_id device, cl_device_info type);

//too many variants to create enum so use raw
std::string getInfo(cl_device_id device, cl_device_info type);

std::vector<cl_platform_id> getGpuPlatforms();
cl_context createContext(cl_platform_id platform);
cl_command_queue createCommandQueue(cl_context context, cl_device_id devices);
cl_mem createOutputBuffer(cl_context context, int size_in_bytes, bool readable = false);

#endif

