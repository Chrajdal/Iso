#include "utils.h"

//-----------------------------------------------------------------------------
template<typename T> T sq_distance(const Tpoint<T>& a, const Tpoint<T>& b)
{
	return ((a.m_x - b.m_x) * (a.m_x - b.m_x))
		+ ((a.m_y - b.m_y) * (a.m_y - b.m_y));
}
void gen_random(std::string& str, const int len)
{
	str = "";
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < len; ++i)
		str += alphanum[rand() % (sizeof(alphanum) - 1)];
}
void gen_randascii(std::string& str, const int len)
{
	str = "";
	static const char arr[] = "!\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	for (int i = 0; i < len; ++i)
		str += arr[rand() % (sizeof(arr) - 1)];
}

//-----------------------------------------------------------------------------