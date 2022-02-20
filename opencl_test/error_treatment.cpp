#include "error_treatment.h"
#include <CL/opencl.h>
#include <string>

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