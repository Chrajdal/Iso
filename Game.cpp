#include "Game.h"
#include "Colors.h"
#include <cmath>
#include <ctime>
#include <vector>

using namespace std;

vector<vector<D3DCOLOR>> g_tile_map;

unsigned int g_size = 100;
double offX = g_size / 2, offY = g_size / 2;
unsigned int g_tile_height = 64;
unsigned int g_tile_width = g_tile_height * 2;

Game::Game( HWND hWnd,KeyboardServer& kServer,const MouseServer& mServer )
:	gfx( hWnd ),
	audio( hWnd ),
	kbd( kServer ),
	mouse( mServer )
{
	srand((unsigned)time(0));

	for (unsigned int i = 0; i < g_size; ++i)
	{
		vector<D3DCOLOR> tmp;
		for (unsigned int j = 0; j < g_size; ++j)
		{
			tmp.push_back(D3DCOLOR_XRGB(rand() % 55, rand() % 200 + 55, rand() % 55));
		}
		g_tile_map.push_back(tmp);
	}

	for (int i = 0; i < 100; ++i)

	g_tile_map[g_size / 2][g_size / 2] = RED;


}

Game::~Game()
{
}

void Game::Go()
{
	UpdateModel();
	gfx.BeginFrame();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
}

void Game::ComposeFrame()
{
	if (kbd.KeyIsPressed(VK_ESCAPE))
		exit(0);

	for (unsigned int i = 0; i < g_tile_map.size(); ++i)
	{
		for (unsigned int j = 0; j < g_tile_map.size(); ++j)
		{
			if (i + offY > 0 && i + offY < g_tile_map.size() - 1 && j + offX > 0 && j + offX < g_tile_map.size() - 1)
			{
				gfx.draw_tile(i - 15, j - 15, g_tile_height, g_tile_width, g_tile_map[i + (int)offY][j + (int)offX]);
			}
		}
	}

	handle_user();

	gfx.draw_rect(gfx.SCREENWIDTH / 2 - 10, gfx.SCREENHEIGHT / 2 - 25, 20, 30, BLUE);
	//gfx.DrawLine(0, 0, gfx.SCREENWIDTH - 1, gfx.SCREENHEIGHT - 1, WHITE);
	//gfx.DrawLine(gfx.SCREENWIDTH - 1, 0, 0, gfx.SCREENHEIGHT - 1, WHITE);
}

void Game::handle_user()
{
	double speed = 0.25;
	if ((kbd.KeyIsPressed(VK_UP) && kbd.KeyIsPressed(VK_DOWN)) ||
		(kbd.KeyIsPressed(VK_UP) && kbd.KeyIsPressed(VK_LEFT)) ||
		(kbd.KeyIsPressed(VK_UP) && kbd.KeyIsPressed(VK_RIGHT)) ||

		(kbd.KeyIsPressed(VK_DOWN) && kbd.KeyIsPressed(VK_LEFT)) ||
		(kbd.KeyIsPressed(VK_DOWN) && kbd.KeyIsPressed(VK_RIGHT)) ||

		(kbd.KeyIsPressed(VK_LEFT) && kbd.KeyIsPressed(VK_RIGHT)))
		return;

	if (kbd.KeyIsPressed(VK_DOWN))
	{
		//if (offY < g_tile_map.size())
			offY += speed;
	}
	if (kbd.KeyIsPressed(VK_UP))
	{
		//if (offY > 0)
			offY -= speed;
	}
	if (kbd.KeyIsPressed(VK_RIGHT))
	{
		//if (offX > 0)
			offX -= speed;
	}
	if (kbd.KeyIsPressed(VK_LEFT))
	{
		//if (offX < g_tile_map.size())
			offX += speed;
	}
	if (kbd.KeyIsPressed(VK_SPACE))
	{
		offX = offY = 0;
	}
}
