#include "cpuDevice.h"



cpuDevice::cpuDevice()
{

	memset(this, 0, sizeof(*this));
	int4 cpu_info;

	__cpuid(&cpu_info.x, 0);
	int nIds_ = cpu_info.x;
	int4* basic_data = (int4*)_malloca((1 + nIds_) * sizeof(int4));
	for (int i = 0; i <= nIds_; ++i)
	{
		__cpuidex(&cpu_info.x, i, 0);
		basic_data[i] = cpu_info;
	}
	char vendor[13];
	memset(vendor, 0, sizeof(vendor));
	*(int*)vendor = basic_data[0].y;
	*(int*)(vendor + 4) = basic_data[0].w;
	*(int*)(vendor + 8) = basic_data[0].z;
	char* ptr_vendor = vendor;
	char* ptr_Intel = "GenuineIntel";
	char* ptr_AMD = "AuthenticAMD";
	size_t i;
	for (i =0; i < 12; i++) {
		if (vendor[i] != ptr_Intel[i])
			break;
	}
	Intel_cpu = i == 12 ? true : false;
	for (i = 0; i < 12; i++) {
		if (vendor[i] != ptr_AMD[i])
			break;
	}
	AMD_cpu = i == 12 ? true : false; 
	int f_1_ECX_;
	int f_1_EDX_;
	int f_7_EBX_;
	int f_7_ECX_;
	int f_81_ECX_;
	int f_81_EDX_;
	if (nIds_ >= 1)
	{
		f_1_ECX_ = basic_data[1].z;
		f_1_EDX_ = basic_data[1].w;
	}
	if (nIds_ >= 7)
	{
		f_7_EBX_ = basic_data[7].y;
		f_7_ECX_ = basic_data[7].z;
	}
	__cpuid(&cpu_info.x, 0x80000000);
	unsigned int ext_nIds_ = cpu_info.x;

	char brand[0x40];
	memset(brand, 0, sizeof(brand));
	int4* extended_data = (int4*)_malloca((1 + ext_nIds_ - 0x80000000) * sizeof(int4));

	for (unsigned int i = 0x80000000, j=0; i <= ext_nIds_; ++i)
	{
		__cpuidex(&cpu_info.x, i, 0);
		extended_data[j++] = cpu_info;
	}
	if (ext_nIds_ >= 0x80000001)
	{
		f_81_ECX_ = extended_data[1].z;
		f_81_EDX_ = extended_data[1].w;
	}
	if (ext_nIds_ >= 0x80000004)
	{
		memcpy(brand, &extended_data[2].x, sizeof(cpu_info));
		memcpy(brand + 16, &extended_data[3].x, sizeof(cpu_info));
		memcpy(brand + 32, &extended_data[4].x, sizeof(cpu_info));
	}
	support_mmx = ((f_1_EDX_ >> 23) & 1);
	support_mmxext = AMD_cpu && ((f_81_EDX_ >> 22) & 1);
	support_avx512CD = ((f_7_EBX_ >> 28) & 1);
	support_avx512ER = ((f_7_EBX_ >> 27) & 1);
	support_avx512PF = ((f_7_EBX_ >> 26) & 1);
	support_avx512F = ((f_7_EBX_ >> 16) & 1);
	support_avx2 = ((f_7_EBX_ >> 5) & 1);
	support_avx = ((f_1_ECX_ >> 28) & 1);
	support_sse1 = ((f_1_EDX_ >> 25) & 1);
	support_sse2 = ((f_1_EDX_ >> 26) & 1);
	support_sse3 = ((f_1_ECX_) & 1);
	support_ssse3 = ((f_1_ECX_ >> 9) & 1);
	support_sse4a = AMD_cpu && ((f_81_ECX_ >> 6) & 1);
	support_sse41 = ((f_1_ECX_ >> 19) & 1);
	support_sse42 = ((f_1_ECX_ >> 20) & 1);
	printf("CPU initialization is complete- %s\n", brand);
	_freea(basic_data);
	_freea(extended_data);
}


cpuDevice::~cpuDevice()
{
}
