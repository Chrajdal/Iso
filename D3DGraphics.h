#pragma once

#include <d3d9.h>
#include "Vec2.h"
#include "Colors.h"

class D3DGraphics
{
public:
	D3DGraphics( HWND hWnd );
	~D3DGraphics();
	void PutPixel( int x,int y,D3DCOLOR c );
	D3DCOLOR GetPixel( int x,int y ) const;
	inline void DrawLine( Vec2& pt1,Vec2& pt2,D3DCOLOR c )
	{
		DrawLine( (int)pt1.x,(int)pt1.y,(int)pt2.x,(int)pt2.y,c );
	}
	void DrawLine( int x1,int y1,int x2,int y2,D3DCOLOR c );
	void DrawCircle( int centerX,int centerY,int radius,D3DCOLOR c );
	void DrawDisc(int centerX, int centerY, int radius, D3DCOLOR color);
	void draw_tile(int x, int y, D3DCOLOR c);
	void draw_rect(int x, int y, int w, int h, D3DCOLOR c);
	
	void BeginFrame();
	void EndFrame();
public:
	static const unsigned int	SCREENWIDTH = 1000;
	static const unsigned int	SCREENHEIGHT = 1000;
private:
	const D3DCOLOR		FILLVALUE =		D3DCOLOR_XRGB(5,5,5);
	IDirect3D9*			pDirect3D;
	IDirect3DDevice9*	pDevice;
	IDirect3DSurface9*	pBackBuffer;
	D3DCOLOR*			pSysBuffer;
};