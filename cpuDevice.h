#pragma once
#include <immintrin.h>
#include <intrin.h>
#include <Windows.h>
#include <stdio.h>
struct int4 {
	int x, y, z, w;
};
class cpuDevice
{
public:
	bool Intel_cpu;
	bool AMD_cpu;
	bool support_avx512F;
	bool support_avx512PF;
	bool support_avx512ER;
	bool support_avx512CD;
	bool support_avx2;
	bool support_avx;
	bool support_sse4a;
	bool support_sse42;
	bool support_sse41;
	bool support_ssse3;
	bool support_sse3;
	bool support_sse2;
	bool support_sse1;
	bool support_mmxext;
	bool support_mmx;
	cpuDevice();
	~cpuDevice();
};

