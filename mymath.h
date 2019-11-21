#pragma once
#include <cmath>

static const float MyPI = std::asinf(1.0f) * 2;

template <typename T>
inline T sq( T val )
{
	return val * val;
}