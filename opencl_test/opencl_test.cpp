#include <CL/opencl.h>
#include <vector>
#include <iostream>
#include <assert.h>




int main()
{
	const int max_buff = 100;
	cl_platform_id platform_infoes_buff[max_buff];
	cl_uint num;
	auto plat_ids_err = clGetPlatformIDs(max_buff, platform_infoes_buff, &num);

	assert(plat_ids_err == CL_SUCCESS);

	const int max_string_len = 2000;


	for (int i = 0; i < num; ++i)
	{
		printf("PLATFORM:\n");

		char platform_name[max_string_len];
		size_t actual_string_len;
		auto plat_info_err = clGetPlatformInfo(platform_infoes_buff[i], CL_PLATFORM_NAME, max_string_len - 1, platform_name, &actual_string_len);
		assert(plat_info_err == CL_SUCCESS);

		platform_name[actual_string_len - 1] = '\0';
		printf("%s\n\n", platform_name);

		cl_device_id devices_infoes_buff[max_buff];
		cl_uint devices_num;
		auto device_ids_err = clGetDeviceIDs(platform_infoes_buff[i], CL_DEVICE_TYPE_ALL, max_buff, devices_infoes_buff, &devices_num);
		//if DEVICE_NOT_FOUND then its just no devices at platform and its not an error so should just go on
		if (device_ids_err == CL_DEVICE_NOT_FOUND)
			continue;

		assert(device_ids_err == CL_SUCCESS);

		printf("DEVICES:\n");

		for (int j = 0; j < devices_num; ++j)
		{
			char device_name[max_string_len];
			size_t device_name_length;
			auto device_info_err = clGetDeviceInfo(devices_infoes_buff[j], CL_DEVICE_NAME, max_buff, device_name, &device_name_length);
			assert(device_info_err == CL_SUCCESS);

			device_name[device_name_length - 1] = '\0';
			printf("%s\n", device_name);
		}
		printf("\n\n");

	}

	

}