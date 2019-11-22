#pragma once

#include <d3d9.h>
#include "Vec2.h"
#include "Colors.h"
#include "Sprite.h"

using namespace olc;

class D3DGraphics
{
public:
	D3DGraphics( HWND hWnd );
	~D3DGraphics();
	void PutColor( int x,int y,D3DCOLOR c );
	D3DCOLOR GetColor( int x,int y ) const;
	inline void DrawLine( Vec2& pt1,Vec2& pt2,D3DCOLOR c )
	{
		DrawLine( (int)pt1.x,(int)pt1.y,(int)pt2.x,(int)pt2.y,c );
	}
	void DrawLine( int x1,int y1,int x2,int y2,D3DCOLOR c );
	void DrawCircle( int centerX,int centerY,int radius,D3DCOLOR c );
	void DrawDisc(int centerX, int centerY, int radius, D3DCOLOR CColor);
	//void draw_tile(int x, int y, D3DCOLOR c);
	void draw_tile(int tile_x, int tile_y, int tile_height, int tile_width, D3DCOLOR c);
	void draw_rect(int x, int y, int w, int h, D3DCOLOR c);
	void draw_rect_fill(int x, int y, int w, int h, D3DCOLOR c);

	void DrawPartialSprite(int32_t x, int32_t y, Sprite* sprite, int32_t ox, int32_t oy, int32_t w, int32_t h, uint32_t scale = 1)
	{
		if (sprite == nullptr)
			return;

		if (scale > 1)
		{
			for (int32_t i = 0; i < w; i++)
				for (int32_t j = 0; j < h; j++)
					for (uint32_t is = 0; is < scale; is++)
						for (uint32_t js = 0; js < scale; js++)
							PutColor(x + (i * scale) + is, y + (j * scale) + js, sprite->GetColor(i + ox, j + oy).dword);
		}
		else
		{
			for (int32_t i = 0; i < w; i++)
				for (int32_t j = 0; j < h; j++)
					PutColor(x + i, y + j, sprite->GetColor(i + ox, j + oy).dword);
		}
	}

	void ConstructFontSheet()
	{
		std::string data;
		data += "?Q`0001oOch0o01o@F40o0<AGD4090LAGD<090@A7ch0?00O7Q`0600>00000000";
		data += "O000000nOT0063Qo4d8>?7a14Gno94AA4gno94AaOT0>o3`oO400o7QN00000400";
		data += "Of80001oOg<7O7moBGT7O7lABET024@aBEd714AiOdl717a_=TH013Q>00000000";
		data += "720D000V?V5oB3Q_HdUoE7a9@DdDE4A9@DmoE4A;Hg]oM4Aj8S4D84@`00000000";
		data += "OaPT1000Oa`^13P1@AI[?g`1@A=[OdAoHgljA4Ao?WlBA7l1710007l100000000";
		data += "ObM6000oOfMV?3QoBDD`O7a0BDDH@5A0BDD<@5A0BGeVO5ao@CQR?5Po00000000";
		data += "Oc``000?Ogij70PO2D]??0Ph2DUM@7i`2DTg@7lh2GUj?0TO0C1870T?00000000";
		data += "70<4001o?P<7?1QoHg43O;`h@GT0@:@LB@d0>:@hN@L0@?aoN@<0O7ao0000?000";
		data += "OcH0001SOglLA7mg24TnK7ln24US>0PL24U140PnOgl0>7QgOcH0K71S0000A000";
		data += "00H00000@Dm1S007@DUSg00?OdTnH7YhOfTL<7Yh@Cl0700?@Ah0300700000000";
		data += "<008001QL00ZA41a@6HnI<1i@FHLM81M@@0LG81?O`0nC?Y7?`0ZA7Y300080000";
		data += "O`082000Oh0827mo6>Hn?Wmo?6HnMb11MP08@C11H`08@FP0@@0004@000000000";
		data += "00P00001Oab00003OcKP0006@6=PMgl<@440MglH@000000`@000001P00000000";
		data += "Ob@8@@00Ob@8@Ga13R@8Mga172@8?PAo3R@827QoOb@820@0O`0007`0000007P0";
		data += "O`000P08Od400g`<3V=P0G`673IP0`@3>1`00P@6O`P00g`<O`000GP800000000";
		data += "?P9PL020O`<`N3R0@E4HC7b0@ET<ATB0@@l6C4B0O`H3N7b0?P01L3R000000020";

		if (fontSprite != nullptr)
			delete fontSprite;
		fontSprite = new olc::Sprite(128, 48);
		int px = 0, py = 0;
		for (int b = 0; b < 1024; b += 4)
		{
			uint32_t sym1 = (uint32_t)data[b + 0] - 48;
			uint32_t sym2 = (uint32_t)data[b + 1] - 48;
			uint32_t sym3 = (uint32_t)data[b + 2] - 48;
			uint32_t sym4 = (uint32_t)data[b + 3] - 48;
			uint32_t r = sym1 << 18 | sym2 << 12 | sym3 << 6 | sym4;

			for (int i = 0; i < 24; i++)
			{
				int k = r & (1 << i) ? 255 : 0;
				fontSprite->SetColor (px, py, CColor(k, k, k, k));
				if (++py == 48) { px++; py = 0; }
			}
		}
	}

	void DrawString(int32_t x, int32_t y, std::string sText, CColor col, uint32_t scale = 1)
	{
		int32_t sx = 0;
		int32_t sy = 0;
		for (auto c : sText)
		{
			if (c == '\n')
			{
				sx = 0; sy += 8 * scale;
			}
			else
			{
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;

				if (scale > 1)
				{
					for (uint32_t i = 0; i < 8; i++)
						for (uint32_t j = 0; j < 8; j++)
							if (fontSprite->GetColor(i + ox * 8, j + oy * 8).GetR() > 0)
								for (uint32_t is = 0; is < scale; is++)
									for (uint32_t js = 0; js < scale; js++)
										PutColor(x + sx + (i * scale) + is, y + sy + (j * scale) + js, col.dword);
				}
				else
				{
					for (uint32_t i = 0; i < 8; i++)
						for (uint32_t j = 0; j < 8; j++)
							if (fontSprite->GetColor(i + ox * 8, j + oy * 8).GetR() > 0)
								PutColor(x + sx + i, y + sy + j, col.dword);
				}
				sx += 8 * scale;
			}
		}
	}
	
	void BeginFrame();
	void EndFrame();
public:
	static const unsigned int	SCREENWIDTH = 1024;
	static const unsigned int	SCREENHEIGHT = 1024;
private:
	const D3DCOLOR 		FILLVALUE = CColors::MakeRGB(225,225,225).dword;
	IDirect3D9*			pDirect3D;
	IDirect3DDevice9*	pDevice;
	IDirect3DSurface9*	pBackBuffer;
	D3DCOLOR *			pSysBuffer;
	Sprite* fontSprite = nullptr;
};
