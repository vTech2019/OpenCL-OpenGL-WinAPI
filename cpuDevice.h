#pragma once
#include <immintrin.h>
#include <Windows.h>
class cpuDevice
{
	bool support_avx;
public:
	cpuDevice();
	~cpuDevice();
};

