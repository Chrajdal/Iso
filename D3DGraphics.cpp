#include "D3DGraphics.h"
#include "mymath.h"
#include <assert.h>
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

	pSysBuffer = new D3DCOLOR[ SCREENWIDTH * SCREENHEIGHT ];
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
	memset( pSysBuffer,FILLVALUE,sizeof(D3DCOLOR)* SCREENWIDTH * SCREENHEIGHT );
}

void D3DGraphics::EndFrame()
{
	HRESULT result;
	D3DLOCKED_RECT backRect;

	result = pBackBuffer->LockRect( &backRect,NULL,NULL );
	assert( !FAILED( result ) );

	for( int y = 0; y < SCREENHEIGHT; y++ )
	{
		memcpy( &((BYTE*)backRect.pBits)[backRect.Pitch * y],&pSysBuffer[SCREENWIDTH * y],sizeof(D3DCOLOR)* SCREENWIDTH );
	}

	result = pBackBuffer->UnlockRect( );
	assert( !FAILED( result ) );

	result = pDevice->Present( NULL,NULL,NULL,NULL );
	assert( !FAILED( result ) );
}

void D3DGraphics::PutPixel( int x,int y,D3DCOLOR c )
{	
	assert( x >= 0 );
	assert( y >= 0 );
	assert( x < SCREENWIDTH );
	assert( y < SCREENHEIGHT );
	pSysBuffer[ x + SCREENWIDTH * y ] = c;
}

D3DCOLOR D3DGraphics::GetPixel( int x,int y ) const
{
	assert( x >= 0 );
	assert( y >= 0 );
	assert( x < SCREENWIDTH );
	assert( y < SCREENHEIGHT );
	return pSysBuffer[ x + SCREENWIDTH * y ];
}

void D3DGraphics::DrawLine( int x1,int y1,int x2,int y2,D3DCOLOR c )
{
	const int dx = x2 - x1;
	const int dy = y2 - y1;

	if( dy == 0 && dx == 0 )
	{
		PutPixel( x1,y1,c );
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
			PutPixel( x,y,c );
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
 			PutPixel( x,y,c );
		}
	}
}

void D3DGraphics::DrawCircle( int centerX,int centerY,int radius,D3DCOLOR color )
{
	int rSquared = sq( radius );
	int xPivot = (int)( radius * 0.70710678118f + 0.5f );
	for( int x = 0; x <= xPivot; x++ )
	{
		int y = (int)(sqrt( (float)( rSquared - sq( x ) ) ) + 0.5f);
		PutPixel( centerX + x,centerY + y,color );
		PutPixel( centerX - x,centerY + y,color );
		PutPixel( centerX + x,centerY - y,color );
		PutPixel( centerX - x,centerY - y,color );
		PutPixel( centerX + y,centerY + x,color );
		PutPixel( centerX - y,centerY + x,color );
		PutPixel( centerX + y,centerY - x,color );
		PutPixel( centerX - y,centerY - x,color );
	}
}

void D3DGraphics::DrawDisc(int centerX, int centerY, int radius, D3DCOLOR color)
{
	int rSquared = sq(radius);
	int xPivot = (int)(radius * 0.70710678118f + 0.5f);
	for (int x = 0; x <= xPivot; x++)
	{
		int y = (int)(sqrt((float)(rSquared - sq(x))) + 0.5f);
		for (int i = 0; i < y; ++i)
		{
			PutPixel(centerX + x, centerY + i, color);
			PutPixel(centerX - x, centerY + i, color);
			PutPixel(centerX + x, centerY - i, color);
			PutPixel(centerX - x, centerY - i, color);
			PutPixel(centerX + i, centerY + x, color);
			PutPixel(centerX - i, centerY + x, color);
			PutPixel(centerX + i, centerY - x, color);
			PutPixel(centerX - i, centerY - x, color);
		}
	}
}

void D3DGraphics::draw_tile(int tile_x, int tile_y, D3DCOLOR c)
{
	int tile_height = 50; // Beware - must be even!
	int tile_width = tile_height * 2;
	int x = (tile_x - tile_y) * tile_width / 2;// +(SCREENWIDTH / 2);
	int y = (tile_x + tile_y) * tile_height / 2;// +(SCREENHEIGHT / 2);

	if (y < 1 || y >= SCREENHEIGHT)
		return;
	if (x < 1 || x >= SCREENWIDTH)
		return;

	for (int i = 0; i < tile_height / 2; ++i)
	{
		for (int j = x - 2 * i; j < x + 2 * i; ++j)
			PutPixel(j, y - tile_height / 2 + i, c);
		for (int j = x - (tile_height - 2 * i); j < x + (tile_height - 2 * i); ++j)
			PutPixel(j, y + i, c);
	}

	/*
	Vec2 top(x, y - tile_height / 2);
	Vec2 right(x + tile_width / 2, y);
	Vec2 left(x - tile_width / 2, y);
	Vec2 down(x, y + tile_height / 2);

	DrawLine(top, right, BLACK);
	DrawLine(top, left, BLACK);
	DrawLine(down, right, BLACK);
	DrawLine(down, left, BLACK);
	*/
}

void D3DGraphics::draw_rect(int x, int y, int w, int h, D3DCOLOR c)
{
	for (int i = y; i < y + h; ++i)
		for (int j = x; j < x + w; ++j)
			PutPixel(j, i, c);
}