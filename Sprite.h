#pragma once

#include <string>
#include "Colors.h"
#include "olcArtifacts.h"
#include "ResourcePack.h"
#include <objidl.h>
#include <gdiplus.h>
#include <d3d9.h>

using namespace Gdiplus;
#pragma comment (lib,"GdiPlus.lib")


static class GDIPlusStartup
{
public:
	GDIPlusStartup()
	{
		Gdiplus::GdiplusStartupInput startupInput;
		ULONG_PTR	token;
		Gdiplus::GdiplusStartup(&token, &startupInput, NULL);
	};
} gdistartup;

// A bitmap-like structure that stores a 2D array of CColors
namespace olc {
	class Sprite
	{
	public:
		Sprite();
		Sprite(std::string sImageFile);
		Sprite(std::string sImageFile, olc::ResourcePack* pack);
		Sprite(const Sprite& o)
			: width(o.width),
			height(o.height),
			modeSample(o.modeSample)
		{
			pColData = new CColor[width*height];
			std::copy_n(o.pColData, width * height, pColData);
		}
		Sprite& operator= (const Sprite& o) {
			if (&o == this)
				return *this;
			width = o.width;
			height = o.height;
			modeSample = o.modeSample;
			pColData = nullptr;
			pColData = new CColor[width * height];
			std::copy_n(o.pColData, width * height, pColData);
			return *this;
		}
		Sprite(int32_t w, int32_t h);
		~Sprite();

	public:
		olc::rcode LoadFromFile(std::string sImageFile, olc::ResourcePack* pack = nullptr);
		olc::rcode LoadFromPGESprFile(std::string sImageFile, olc::ResourcePack* pack = nullptr);
		olc::rcode SaveToPGESprFile(std::string sImageFile);

	public:
		int width = 0;
		int height = 0;
		enum Mode { NORMAL, PERIODIC };

	public:
		//void SetSampleMode(olc::Sprite::Mode mode = olc::Sprite::Mode::NORMAL);
		CColor GetColor(int32_t x, int32_t y);
		bool  SetColor(int32_t x, int32_t y, CColor p);

		CColor Sample(float x, float y);
		CColor SampleBL(float u, float v);
		CColor* GetData();

	private:
		CColor* pColData = nullptr;
		Mode modeSample = Mode::NORMAL;

#ifdef OLC_DBG_OVERDRAW
	public:
		static int nOverdrawCount;
#endif

	};
}