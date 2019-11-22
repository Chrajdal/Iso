#include "Sprite.h"


namespace olc {
	Sprite::Sprite()
	{
		pColData = nullptr;
		width = 0;
		height = 0;
	}

	Sprite::Sprite(std::string sImageFile)
	{
		LoadFromFile(sImageFile);
	}

	Sprite::Sprite(std::string sImageFile, olc::ResourcePack* pack)
	{
		LoadFromPGESprFile(sImageFile, pack);
	}

	Sprite::Sprite(int32_t w, int32_t h)
	{
		if (pColData) delete[] pColData;
		width = w;		height = h;
		pColData = new CColor[width * height];
		for (int32_t i = 0; i < width * height; i++)
			pColData[i] = CColor();
	}

	Sprite::~Sprite()
	{
		if (pColData) delete pColData;
	}

	olc::rcode Sprite::LoadFromPGESprFile(std::string sImageFile, olc::ResourcePack* pack)
	{
		if (pColData) delete[] pColData;

		auto ReadData = [&](std::istream& is)
		{
			is.read((char*)& width, sizeof(int32_t));
			is.read((char*)& height, sizeof(int32_t));
			pColData = new CColor[width * height];
			is.read((char*)pColData, width * height * sizeof(uint32_t));
		};

		// These are essentially Memory Surfaces represented by olc::Sprite
		// which load very fast, but are completely uncompressed
		if (pack == nullptr)
		{
			std::ifstream ifs;
			ifs.open(sImageFile, std::ifstream::binary);
			if (ifs.is_open())
			{
				ReadData(ifs);
				return olc::OK;
			}
			else
				return olc::FAIL;
		}
		else
		{
			auto streamBuffer = pack->GetStreamBuffer(sImageFile);
			std::istream is(&streamBuffer);
			ReadData(is);
		}


		return olc::FAIL;
	}

	olc::rcode Sprite::SaveToPGESprFile(std::string sImageFile)
	{
		if (pColData == nullptr) return olc::FAIL;

		std::ofstream ofs;
		ofs.open(sImageFile, std::ifstream::binary);
		if (ofs.is_open())
		{
			ofs.write((char*)& width, sizeof(int32_t));
			ofs.write((char*)& height, sizeof(int32_t));
			ofs.write((char*)pColData, width * height * sizeof(uint32_t));
			ofs.close();
			return olc::OK;
		}

		return olc::FAIL;
	}

	olc::rcode Sprite::LoadFromFile(std::string sImageFile, olc::ResourcePack* pack)
	{
		//UNUSED(pack);
#if defined(_WIN32)
		// Use GDI+
		std::wstring wsImageFile = std::wstring(sImageFile.begin(), sImageFile.end());
		Gdiplus::Bitmap* bmp = Gdiplus::Bitmap::FromFile(wsImageFile.c_str());
		if (bmp == nullptr)
			return olc::NO_FILE;

		width = bmp->GetWidth();
		height = bmp->GetHeight();
		pColData = new CColor[width * height];

		for (int x = 0; x < width; x++)
			for (int y = 0; y < height; y++)
			{
				Gdiplus::Color c;
				bmp->GetPixel(x, y, &c);
				if (c.GetAlpha())
					continue;
				SetColor(x, y, CColors::MakeRGB(c.GetRed(), c.GetGreen(), c.GetBlue()/*, c.GetAlpha()*/)); // deliberately loosing alpha value for now
			}
		delete bmp;
		return olc::OK;
#endif

#if defined(__linux__)
		////////////////////////////////////////////////////////////////////////////
		// Use libpng, Thanks to Guillaume Cottenceau
		// https://gist.github.com/niw/5963798
		png_structp png;
		png_infop info;

		FILE* f = fopen(sImageFile.c_str(), "rb");
		if (!f) return olc::NO_FILE;

		png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png) goto fail_load;

		info = png_create_info_struct(png);
		if (!info) goto fail_load;

		if (setjmp(png_jmpbuf(png))) goto fail_load;

		png_init_io(png, f);
		png_read_info(png, info);

		png_byte CColor_type;
		png_byte bit_depth;
		png_bytep* row_pointers;
		width = png_get_image_width(png, info);
		height = png_get_image_height(png, info);
		CColor_type = png_get_CColor_type(png, info);
		bit_depth = png_get_bit_depth(png, info);

#ifdef _DEBUG
		std::cout << "Loading PNG: " << sImageFile << "\n";
		std::cout << "W:" << width << " H:" << height << " D:" << (int)bit_depth << "\n";
#endif

		if (bit_depth == 16) png_set_strip_16(png);
		if (CColor_type == PNG_CColor_TYPE_PALETTE) png_set_palette_to_rgb(png);
		if (CColor_type == PNG_CColor_TYPE_GRAY && bit_depth < 8)	png_set_expand_gray_1_2_4_to_8(png);
		if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
		if (CColor_type == PNG_CColor_TYPE_RGB ||
			CColor_type == PNG_CColor_TYPE_GRAY ||
			CColor_type == PNG_CColor_TYPE_PALETTE)
			png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
		if (CColor_type == PNG_CColor_TYPE_GRAY ||
			CColor_type == PNG_CColor_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(png);

		png_read_update_info(png, info);
		row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
		for (int y = 0; y < height; y++) {
			row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
		}
		png_read_image(png, row_pointers);
		////////////////////////////////////////////////////////////////////////////

		// Create sprite array
		pColData = new CColor[width * height];

		// Iterate through image rows, converting into sprite format
		for (int y = 0; y < height; y++)
		{
			png_bytep row = row_pointers[y];
			for (int x = 0; x < width; x++)
			{
				png_bytep px = &(row[x * 4]);
				SetCColor(x, y, CColor(px[0], px[1], px[2], px[3]));
			}
		}

		fclose(f);
		return olc::OK;

	fail_load:
		width = 0;
		height = 0;
		fclose(f);
		pColData = nullptr;
		return olc::FAIL;
#endif
	}

	//void Sprite::SetSampleMode(olc::Sprite::Mode mode)
	//{
	//	modeSample = mode;
	//}


	CColor Sprite::GetColor(int32_t x, int32_t y)
	{
		if (modeSample == olc::Sprite::Mode::NORMAL)
		{
			if (x >= 0 && x < width && y >= 0 && y < height)
				return pColData[y * width + x];
			else
				return CColor(0, 0, 0, 0);
		}
		else
		{
			return pColData[abs(y % height) * width + abs(x % width)];
		}
	}

	bool Sprite::SetColor(int32_t x, int32_t y, CColor p)
	{

#ifdef OLC_DBG_OVERDRAW
		nOverdrawCount++;
#endif

		if (x >= 0 && x < width && y >= 0 && y < height)
		{
			pColData[y * width + x] = p;
			return true;
		}
		else
			return false;
	}

	CColor Sprite::Sample(float x, float y)
	{
		int32_t sx = std::min((int32_t)((x * (float)width)), width - 1);
		int32_t sy = std::min((int32_t)((y * (float)height)), height - 1);
		return GetColor(sx, sy);
	}

	CColor Sprite::SampleBL(float u, float v)
	{
		u = u * width - 0.5f;
		v = v * height - 0.5f;
		int x = (int)floor(u); // cast to int rounds toward zero, not downward
		int y = (int)floor(v); // Thanks @joshinils
		float u_ratio = u - x;
		float v_ratio = v - y;
		float u_opposite = 1 - u_ratio;
		float v_opposite = 1 - v_ratio;

		CColor p1 = GetColor(std::max(x, 0), std::max(y, 0));
		CColor p2 = GetColor(std::min(x + 1, (int)width - 1), std::max(y, 0));
		CColor p3 = GetColor(std::max(x, 0), std::min(y + 1, (int)height - 1));
		CColor p4 = GetColor(std::min(x + 1, (int)width - 1), std::min(y + 1, (int)height - 1));

		return CColor(
			(uint8_t)((p1.GetR() * u_opposite + p2.GetR() * u_ratio) * v_opposite + (p3.GetR() * u_opposite + p4.GetR() * u_ratio) * v_ratio),
			(uint8_t)((p1.GetG() * u_opposite + p2.GetG() * u_ratio) * v_opposite + (p3.GetG() * u_opposite + p4.GetG() * u_ratio) * v_ratio),
			(uint8_t)((p1.GetB() * u_opposite + p2.GetB() * u_ratio) * v_opposite + (p3.GetB() * u_opposite + p4.GetB() * u_ratio) * v_ratio));
	}

	CColor* Sprite::GetData() { return pColData; }
}