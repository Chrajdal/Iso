#pragma once

class CColor
{
public:
	unsigned int dword;
public:
	constexpr CColor() : dword() {}
	constexpr CColor(const CColor& col)
		:
		dword(col.dword)
	{}
	constexpr CColor(unsigned int dw)
		:
		dword(dw)
	{}
	constexpr CColor(unsigned char x, unsigned char r, unsigned char g, unsigned char b)
		:
		dword((x << 24u) | (r << 16u) | (g << 8u) | b)
	{}
	constexpr CColor(unsigned char r, unsigned char g, unsigned char b)
		:
		dword((r << 16u) | (g << 8u) | b)
	{}
	constexpr CColor(CColor col, unsigned char x)
		:
		CColor((x << 24u) | col.dword)
	{}
	CColor& operator =(CColor CColor)
	{
		dword = CColor.dword;
		return *this;
	}
	inline bool operator ==(const CColor& rhs) const
	{
		return dword == rhs.dword;
		//return GetR() == rhs.GetR() && GetG() == rhs.GetG() && GetB() == rhs.GetB();
	}
	inline bool operator !=(const CColor& rhs) const
	{
		return dword != rhs.dword;
		//return GetR() != rhs.GetR() || GetG() != rhs.GetG() || GetB() != rhs.GetB();
	}
	constexpr unsigned char GetX() const
	{
		return dword >> 24u;
	}
	constexpr unsigned char GetA() const
	{
		return GetX();
	}
	constexpr unsigned char GetR() const
	{
		return (dword >> 16u) & 0xFFu;
	}
	constexpr unsigned char GetG() const
	{
		return (dword >> 8u) & 0xFFu;
	}
	constexpr unsigned char GetB() const
	{
		return dword & 0xFFu;
	}
	void SetX(unsigned char x)
	{
		dword = (dword & 0xFFFFFFu) | (x << 24u);
	}
	void SetA(unsigned char a)
	{
		SetX(a);
	}
	void SetR(unsigned char r)
	{
		dword = (dword & 0xFF00FFFFu) | (r << 16u);
	}
	void SetG(unsigned char g)
	{
		dword = (dword & 0xFFFF00FFu) | (g << 8u);
	}
	void SetB(unsigned char b)
	{
		dword = (dword & 0xFFFFFF00u) | b;
	}
};

namespace CColors
{
	static constexpr CColor MakeRGB(unsigned char r, unsigned char g, unsigned char b)
	{
		return (r << 16) | (g << 8) | b;
	}
	static constexpr CColor White = MakeRGB(255u, 255u, 255u);
	static constexpr CColor Black = MakeRGB(0u, 0u, 0u);
	static constexpr CColor Gray = MakeRGB(0x80u, 0x80u, 0x80u);
	static constexpr CColor LightGray = MakeRGB(0xD3u, 0xD3u, 0xD3u);
	static constexpr CColor Red = MakeRGB(255u, 0u, 0u);
	static constexpr CColor Green = MakeRGB(0u, 255u, 0u);
	static constexpr CColor Blue = MakeRGB(0u, 0u, 255u);
	static constexpr CColor Yellow = MakeRGB(255u, 255u, 0u);
	static constexpr CColor Cyan = MakeRGB(0u, 255u, 255u);
	static constexpr CColor Magenta = MakeRGB(255u, 0u, 255u);
	static constexpr CColor Gold = MakeRGB(255u, 215u, 0u);
}