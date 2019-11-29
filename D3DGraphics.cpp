#include "D3DGraphics.h"
#include "mymath.h"
#include <cassert>
#pragma comment( lib,"d3d9.lib" )


D3DGraphics::D3DGraphics( HWND hWnd )
	:
pDirect3D( NULL ),
pDevice( NULL ),
pBackBuffer( NULL ),
pSysBuffer( NULL )
{
	HRESULT result;
	
	pDirect3D = Direct3DCreate9( D3D_SDK_VERSION );
	assert( pDirect3D != NULL );

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp,sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

    result = pDirect3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE,&d3dpp,&pDevice );
	assert( !FAILED( result ) );

	result = pDevice->GetBackBuffer( 0,0,D3DBACKBUFFER_TYPE_MONO,&pBackBuffer );
	assert( !FAILED( result ) );

	pSysBuffer = new D3DCOLOR [ SCREENWIDTH * SCREENHEIGHT ];

	ConstructFontSheet();
}

D3DGraphics::~D3DGraphics()
{
	if( pDevice )
	{
		pDevice->Release();
		pDevice = NULL;
	}
	if( pDirect3D )
	{
		pDirect3D->Release();
		pDirect3D = NULL;
	}
	if( pBackBuffer )
	{
		pBackBuffer->Release();
		pBackBuffer = NULL;
	}
	if( pSysBuffer )
	{
		delete pSysBuffer;
		pSysBuffer = NULL;
	}
}

void D3DGraphics::BeginFrame()
{
	//memset( pSysBuffer,FILLVALUE,sizeof(D3DCOLOR )* SCREENWIDTH * SCREENHEIGHT );
	std::fill_n(pSysBuffer, SCREENWIDTH * SCREENHEIGHT, CColors::MakeRGB(55,55,55).dword);
}

void D3DGraphics::EndFrame()
{
	HRESULT result;
	D3DLOCKED_RECT backRect;

	result = pBackBuffer->LockRect( &backRect,NULL,NULL );
	assert( !FAILED( result ) );

	for( int y = 0; y < SCREENHEIGHT; y++ )
	{
		memcpy( &((BYTE*)backRect.pBits)[backRect.Pitch * y],&pSysBuffer[SCREENWIDTH * y],sizeof(D3DCOLOR )* SCREENWIDTH );
	}

	result = pBackBuffer->UnlockRect( );
	assert( !FAILED( result ) );

	result = pDevice->Present( NULL,NULL,NULL,NULL );
	assert( !FAILED( result ) );
}

void D3DGraphics::PutColor( int x,int y,D3DCOLOR c )
{
	if ((x >= 0)
		&& (y >= 0)
		&& (x < SCREENWIDTH)
		&& (y < SCREENHEIGHT)) {
		CColor theColor = c;
		if (m_mode == PixelMode::ALPHA)
		{
			CColor d = GetColor(x,y);
			CColor col = c;
			float a = (float)(col.GetA() / 255.0f) * 1.0f;
			float c = 1.0f - a;
			float r = a * (float)col.GetR() + c * (float)d.GetR();
			float g = a * (float)col.GetG() + c * (float)d.GetG();
			float b = a * (float)col.GetB() + c * (float)d.GetB();
			theColor = CColor((uint8_t)r, (uint8_t)g, (uint8_t)b);
		}
		pSysBuffer[x + SCREENWIDTH * y] = theColor.dword;
	}
}

D3DCOLOR  D3DGraphics::GetColor( int x,int y ) const
{
	assert( x >= 0 );
	assert( y >= 0 );
	assert( x < SCREENWIDTH );
	assert( y < SCREENHEIGHT );
	return pSysBuffer[ x + SCREENWIDTH * y ];
}

void D3DGraphics::DrawLine( int x1,int y1,int x2,int y2,D3DCOLOR  c )
{
	const int dx = x2 - x1;
	const int dy = y2 - y1;

	if( dy == 0 && dx == 0 )
	{
		PutColor( x1,y1,c );
	}
	else if( abs( dy ) > abs( dx ) )
	{
		if( dy < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		const float m = (float)dx / (float)dy;
		const float b = x1 - m*y1;
		for( int y = y1; y <= y2; y = y + 1 )
		{
			int x = (int)(m*y + b + 0.5f);
			PutColor( x,y,c );
		}
	}
	else
	{
		if( dx < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		const float m = (float)dy / (float)dx;
		const float b = y1 - m*x1;
		for( int x = x1; x <= x2; x = x + 1 )
		{
			int y = (int)(m*x + b + 0.5f);
 			PutColor( x,y,c );
		}
	}
}

void D3DGraphics::DrawCircle( int centerX,int centerY,int radius,D3DCOLOR  CColor )
{
	int rSquared = sq( radius );
	int xPivot = (int)( radius * 0.70710678118f + 0.5f );
	for( int x = 0; x <= xPivot; x++ )
	{
		int y = (int)(sqrt( (float)( rSquared - sq( x ) ) ) + 0.5f);
		PutColor( centerX + x,centerY + y,CColor );
		PutColor( centerX - x,centerY + y,CColor );
		PutColor( centerX + x,centerY - y,CColor );
		PutColor( centerX - x,centerY - y,CColor );
		PutColor( centerX + y,centerY + x,CColor );
		PutColor( centerX - y,centerY + x,CColor );
		PutColor( centerX + y,centerY - x,CColor );
		PutColor( centerX - y,centerY - x,CColor );
	}
}

void D3DGraphics::DrawDisc(int centerX, int centerY, int radius, D3DCOLOR  CColor)
{
	int rSquared = sq(radius);
	int xPivot = (int)(radius * 0.70710678118f + 0.5f);
	for (int x = 0; x <= xPivot; x++)
	{
		int y = (int)(sqrt((float)(rSquared - sq(x))) + 0.5f);
		for (int i = 0; i < y; ++i)
		{
			PutColor(centerX + x, centerY + i, CColor);
			PutColor(centerX - x, centerY + i, CColor);
			PutColor(centerX + x, centerY - i, CColor);
			PutColor(centerX - x, centerY - i, CColor);
			PutColor(centerX + i, centerY + x, CColor);
			PutColor(centerX - i, centerY + x, CColor);
			PutColor(centerX + i, centerY - x, CColor);
			PutColor(centerX - i, centerY - x, CColor);
		}
	}
}

void D3DGraphics::DrawRect(int x, int y, int w, int h, D3DCOLOR c)
{
	DrawLine(x, y, x, y + h, c);
	DrawLine(x, y, x + w, y, c);
	DrawLine(x + w, y, x + w, y + h, c);
	DrawLine(x, y + h, x + w, y + h, c);
}

void D3DGraphics::DrawRectFill(int x, int y, int w, int h, D3DCOLOR c)
{
	for (int i = y; i < y + h; ++i)
		for (int j = x; j < x + w; ++j)
			PutColor(j, i, c);
}

void D3DGraphics::DrawSprite(int x, int y, Sprite* sprite, unsigned scale)
{
	if (sprite == nullptr)
		return;
	if (scale > 1)
	{
		for (int i = 0; i < sprite->width; i++)
			for (int j = 0; j < sprite->height; j++)
				for (unsigned is = 0; is < scale; is++)
					for (unsigned js = 0; js < scale; js++)
						PutColor(x + (i * scale) + is, y + (j * scale) + js, sprite->GetColor(i, j).dword);
	}
	else
	{
		if(m_mode == PixelMode::NORMAL)
		for (int i = 0; i < sprite->height; ++i)
			std::copy_n((D3DCOLOR*)(sprite->GetData()) + i * sprite->width, sprite->width, pSysBuffer + (y + i) * SCREENWIDTH + x);
		if (m_mode == PixelMode::ALPHA)
			for (int i = 0; i < sprite->width; i++)
				for (int j = 0; j < sprite->height; j++)
					PutColor(x + i, y + j, sprite->GetColor(i, j).dword);
	}
}

void D3DGraphics::DrawPartialSprite(int x, int y, Sprite* sprite, int ox, int oy, int w, int h, unsigned scale)
{
	if (sprite == nullptr || scale == 0)
		return;
	if (scale > 1)
	{
		for (int i = 0; i < w; i++)
			for (int j = 0; j < h; j++)
				for (unsigned is = 0; is < scale; is++)
					for (unsigned js = 0; js < scale; js++)
						PutColor(x + (i * scale) + is, y + (j * scale) + js, sprite->GetColor(i + ox, j + oy).dword);
	}
	else
	{
		for (int i = 0; i < w; i++)
			for (int j = 0; j < h; j++)
				PutColor(x + i, y + j, sprite->GetColor(i + ox, j + oy).dword);
	}
}
