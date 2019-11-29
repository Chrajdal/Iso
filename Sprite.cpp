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
				return olc::rcode::OK;
			}
			else
				return olc::rcode::FAIL;
		}
		else
		{
			auto streamBuffer = pack->GetStreamBuffer(sImageFile);
			std::istream is(&streamBuffer);
			ReadData(is);
		}

		return olc::rcode::FAIL;
	}

	olc::rcode Sprite::SaveToPGESprFile(std::string sImageFile)
	{
		if (pColData == nullptr) return olc::rcode::FAIL;

		std::ofstream ofs;
		ofs.open(sImageFile, std::ifstream::binary);
		if (ofs.is_open())
		{
			ofs.write((char*)& width, sizeof(int32_t));
			ofs.write((char*)& height, sizeof(int32_t));
			ofs.write((char*)pColData, width * height * sizeof(uint32_t));
			ofs.close();
			return olc::rcode::OK;
		}

		return olc::rcode::FAIL;
	}

	olc::rcode Sprite::LoadFromFile(std::string sImageFile, olc::ResourcePack* pack)
	{
		std::wstring wsImageFile = std::wstring(sImageFile.begin(), sImageFile.end());
		Gdiplus::Bitmap* bmp = Gdiplus::Bitmap::FromFile(wsImageFile.c_str());
		if (bmp == nullptr)
			return olc::rcode::NO_FILE;

		width = bmp->GetWidth();
		height = bmp->GetHeight();

		std::cout << width << " / " << height << "\n";
		
		pColData = new CColor[width * height];
		for (int x = 0; x < width; x++)
			for (int y = 0; y < height; y++)
			{
				Gdiplus::Color c;
				bmp->GetPixel(x, y, &c);
				CColor col{ c.GetAlpha(), c.GetRed(), c.GetGreen(), c.GetBlue()  };
				SetColor(x, y, col);
			}
		delete bmp;
		return olc::rcode::OK;
	}

	CColor Sprite::GetColor(int32_t x, int32_t y)
	{
		if (modeSample == olc::Sprite::Mode::NORMAL)
		{
			if (x >= 0 && x < width && y >= 0 && y < height)
				return pColData[y * width + x];
			else
				return  CColors::MakeRGB(0, 0, 0);
		}
		else
		{
			return pColData[abs(y % height) * width + abs(x % width)];
		}
	}

	bool Sprite::SetColor(int32_t x, int32_t y, CColor p)
	{
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

		return CColors::MakeRGB(
			(uint8_t)((p1.GetR() * u_opposite + p2.GetR() * u_ratio) * v_opposite + (p3.GetR() * u_opposite + p4.GetR() * u_ratio) * v_ratio),
			(uint8_t)((p1.GetG() * u_opposite + p2.GetG() * u_ratio) * v_opposite + (p3.GetG() * u_opposite + p4.GetG() * u_ratio) * v_ratio),
			(uint8_t)((p1.GetB() * u_opposite + p2.GetB() * u_ratio) * v_opposite + (p3.GetB() * u_opposite + p4.GetB() * u_ratio) * v_ratio));
	}

	CColor* Sprite::GetData() { return pColData; }

}