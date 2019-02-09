#include "cpuDevice.h"



cpuDevice::cpuDevice()
{
	int cpu_info[4];
	int functions = 1;
	int sub_leaf = 0;
	__cpuidex(cpu_info, functions, sub_leaf);
	support_avx = cpu_info[2] & (1 << 28) || false;
}


cpuDevice::~cpuDevice()
{
}
