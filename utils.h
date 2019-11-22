#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include <chrono>

//-----------------------------------------------------------------------------
template<typename T>
struct Tpoint
{
	Tpoint(const T& _x = 0, const T & _y = 0)
		: m_x(_x), m_y(_y)
	{}
	Tpoint(const Tpoint<T>& src)
		: m_x(src.m_x), m_y(src.m_y)
	{}
	Tpoint(const Tpoint<T>&& src)
		: m_x(move(src.m_x)), m_y(move(src.m_y))
	{}
	Tpoint<T>& operator = (const Tpoint<T>& src)
	{
		if (this == &src)
			return *this;
		m_x = src.m_x;
		m_y = src.m_y;
		return *this;
	}
	Tpoint<T>& operator = (const Tpoint<T>&& src)
	{
		if (this == &src)
			return *this;
		m_x = move(src.m_x);
		m_y = move(src.m_y);
		return *this;
	}
	inline bool operator == (const Tpoint<T>& rhs) const
	{
		return m_x == rhs.m_x && m_y == rhs.m_y;
	}
	inline bool operator != (const Tpoint<T>& rhs) const
	{
		return !(*this == rhs);
	}
	T m_x;
	T m_y;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T>
struct Trect
{
	Trect(const Tpoint<T>& upleft = {}, const Tpoint<T>& downright = {})
		: m_upleft(upleft), m_downright(downright)
	{}
	Trect(const Trect<T>& src)
		: m_upleft(src.m_upleft), m_downright(src.m_downright)
	{}
	Trect(const Trect<T>&& src)
		: m_upleft(move(src.m_upleft)), m_downright(move(src.m_downright))
	{}
	Trect<T>& operator = (const Trect<T>& src)
	{
		if (this == &src)
			return *this;
		m_upleft = src.m_upleft;
		m_downright = src.m_downright;
		return *this;
	}
	Trect<T>& operator = (const Trect<T>&& src)
	{
		if (this == &src)
			return *this;
		m_upleft = move(src.m_upleft);
		m_downright = move(src.m_downright);
		return *this;
	}
	Tpoint<T> m_upleft;
	Tpoint<T> m_downright;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T> T sq_distance(const Tpoint<T>& a, const Tpoint<T>& b);
void gen_random(std::string& str, const int len);
void gen_randascii(std::string& str, const int len);
//-----------------------------------------------------------------------------