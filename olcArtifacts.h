#pragma once

#include <algorithm>
using std::min;
using std::max;
#define NOMINMAX
#include <Windows.h>

namespace olc {
	enum class rcode
	{
		FAIL = 0,
		OK = 1,
		NO_FILE = -1,
	};
}
