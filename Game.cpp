#include "Game.h"
#include "Colors.h"
#include <cmath>
#include <ctime>
#include <vector>

using namespace std;

/*
hello
*/

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

// yay using algebra outside of school!
void tile_to_screen_position(int tile_x, int tile_y, int & screen_x, int & screen_y, int tile_w, int tile_h, int screen_w, int screen_h)
{
	screen_x = (tile_x - tile_y) * tile_w / 2;
	screen_y = (tile_x + tile_y) * tile_h / 2;
}
void screen_to_tile_position(int & tile_x, int & tile_y, int screen_x, int screen_y, int tile_w, int tile_h, int screen_w, int screen_h)
{
	tile_x = (screen_x / (tile_w / 2) + screen_y / (tile_h / 2)) / 2;
	tile_y = (screen_y / (tile_h / 2) - (screen_x / (tile_w / 2))) / 2;
}

void Game::ComposeFrame()
{
	if (kbd.KeyIsPressed(VK_ESCAPE))
		exit(0);

	int start_tile_x, start_tile_y, end_tile_x, end_tile_y;
	screen_to_tile_position(start_tile_x, start_tile_y, 0, 0, g_tile_width, g_tile_height, gfx.SCREENWIDTH, gfx.SCREENHEIGHT);
	screen_to_tile_position(end_tile_x, end_tile_y, gfx.SCREENWIDTH - 1, gfx.SCREENHEIGHT - 1, g_tile_width, g_tile_height, gfx.SCREENWIDTH, gfx.SCREENHEIGHT);

	for (unsigned int i = start_tile_y; i < end_tile_y; ++i)
	{
		for (unsigned int j = start_tile_x; j < end_tile_x; ++j)
		{
			if (i + offY > 0 && i + offY < g_tile_map.size() - 1 && j + offX > 0 && j + offX < g_tile_map.size() - 1)
			{
				gfx.draw_tile(i, j, g_tile_height, g_tile_width, g_tile_map[j + offX][i + offY]);
			}
		}
	}

	handle_user();
	gfx.draw_rect(gfx.SCREENWIDTH / 2 - 10, gfx.SCREENHEIGHT / 2 - 25, 20, 30, BLUE);
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
