#pragma once

#include <chrono>

class Timer
{
public:
	Timer(void)
	{
		start = std::chrono::high_resolution_clock::now();
	}
	inline double elapsed(void) const
	{
		return (double)(std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::high_resolution_clock::now() - start)).count();
	}
	inline double elapsedns(void) const
	{
		return (double)(std::chrono::duration_cast<std::chrono::nanoseconds>
			(std::chrono::high_resolution_clock::now() - start)).count();
	}
	inline void restart(void)
	{
		start = std::chrono::high_resolution_clock::now();
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};
