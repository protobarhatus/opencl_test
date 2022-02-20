#include <CL/opencl.h>
#include <vector>
#include <iostream>
#include <assert.h>
#include <random>
#include <string>
#include "error_treatment.h"
#include "openclwrap.h"
#include "useful_vector_funcs.h"
#include "code_load.h"
void printPlatformsAndDevices();
#define STRINGIFY(...) #__VA_ARGS__




//is there any ways to get string representation of GemmType behind typedef?
//like STRINGIGY(GemmType) but returns "float" not "GemmType"
typedef float GemmType;
#define STRING_TYPE "float"

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

	auto A = randMatrix<GemmType>(X, AY), B = randMatrix< GemmType>(AY, Y), C = randMatrix< GemmType>(X, Y);

	GemmType alpha = 0.5, beta = 0.7;
	//bigger is not possible since iris xe gives CL_DEVICE_MAX_WORK_GROUP_SIZE as 256
	//Actually, with narrowing int->size_t it DID compile and even without warnings
	//but im using MSVC now
	size_t TS = 10;

	cl_mem Abuff = vectorToClBuffer<GemmType>(context, queue, A, true);
	cl_mem Bbuff = vectorToClBuffer<GemmType>(context, queue, B, true);

	//cl_mem Cbuff = createOutputBuffer(context, A.size() * B[0].size() * sizeof(int), true);
	cl_mem Cbuff = vectorToClBuffer<GemmType>(context, queue, C, true, true);


	std::string code = loadCode("gemm.h");
	code = "#define TS " + std::to_string(TS) + "\n" + code;
	code = (std::string)"#define _TYPE " + std::string(STRING_TYPE) + "\n" + code;
	const char* codechar = code.data();

	cl_program program = clCreateProgramWithSource(context, 1, &codechar, NULL, NULL);
	int ret = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (ret == CL_BUILD_PROGRAM_FAILURE)
	{
		char log[10000];
		clGetProgramBuildInfo(program, getDevices(platforms[0], DEVICE_TYPE_GPU)[0], CL_PROGRAM_BUILD_LOG, 10000, log, NULL);
		printf("%s", log);
		system("pause");
	}
	
	cl_kernel kernel = clCreateKernel(program, "gemm", NULL);

	setKernelArg(kernel, 0, Abuff);
	setKernelArg(kernel, 1, Bbuff);
	setKernelArg(kernel, 2, Cbuff);

	setKernelArg(kernel, 3, A.size());
	setKernelArg(kernel, 4, A[0].size());
	setKernelArg(kernel, 5, B[0].size());
	setKernelArg(kernel, 6, alpha);
	setKernelArg(kernel, 7, beta);

	size_t work_size[2] = { A.size()  , B[0].size() };

	size_t local_group[2] = { TS, TS };

	time_t t1, t2;
	t1 = clock();

	int res = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, work_size, local_group, 0, NULL, NULL);

	if (!isOk(res))
		throw stringError(res);


	auto Creaden = readClBufferAsMatrix<GemmType>(Cbuff, queue, A.size(), B[0].size());
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
		std::cout << C << "\n\n" << alpha*A*B + beta*C << "\n\n" << Creaden;
		//std::cout << "B from Buff" << readClBufferAsMatrix<int>(Bbuff, queue, B.size(), B[0].size());
	}

	clReleaseMemObject(Abuff);
	clReleaseMemObject(Bbuff);
	clReleaseMemObject(Cbuff);

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
