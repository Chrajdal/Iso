#include "Game.h"
#include "Colors.h"
#include <cmath>
#include <ctime>
#include <vector>

using namespace std;

vector<vector<D3DCOLOR>> g_tile_map;

unsigned int g_size = 100;

// -----------------------------------------
// error this does not work this way
double offX = g_size / 2, offY = g_size / 2;
// -----------------------------------------

unsigned int g_tile_height = 50;
unsigned int g_tile_width = g_tile_height * 2;

int a1, a2, b1, b2, c1, c2, d1, d2;

// yay using algebra outside of school!
void tile_to_screen_position(int tile_x, int tile_y, int & screen_x, int & screen_y, int tile_w, int tile_h)
{
	screen_x = (tile_x - tile_y) * tile_w / 2;
	screen_y = (tile_x + tile_y) * tile_h / 2;
}
void screen_to_tile_position(int & tile_x, int & tile_y, int screen_x, int screen_y, int tile_w, int tile_h)
{
	tile_x = double(screen_x) / tile_w + double(screen_y) / tile_h;
	tile_y = double(screen_y) / tile_h - double(screen_x) / tile_w;
}

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

	screen_to_tile_position(a1, a2, 0, 0, g_tile_width, g_tile_height);
	screen_to_tile_position(b1, b2, gfx.SCREENWIDTH, 0, g_tile_width, g_tile_height);
	screen_to_tile_position(c1, c2, 0, gfx.SCREENHEIGHT, g_tile_width, g_tile_height);
	screen_to_tile_position(d1, d2, gfx.SCREENWIDTH, gfx.SCREENHEIGHT, g_tile_width, g_tile_height);
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

	int startx = a1;
	int starty = b2;
	int endx = d1;
	int endy = c2;
	for (int i = startx; i <= endx; ++i)
		for (int j = starty; j <= endy; ++j)
			if (i + offY >= 0 && i + offY < g_tile_map.size() && j + offX >= 0 && j + offX < g_tile_map.size())
				gfx.draw_tile(i, j, g_tile_height, g_tile_width, g_tile_map[j + offX][i + offY]);

	gfx.draw_rect(gfx.SCREENWIDTH / 2 - 10, gfx.SCREENHEIGHT / 2 - 25, 20, 30, BLUE);

	gfx.draw_rect(0, 0, g_tile_width / 2, gfx.SCREENHEIGHT, BLACK);
	gfx.draw_rect(gfx.SCREENWIDTH - g_tile_width / 2, 0, g_tile_width / 2, gfx.SCREENHEIGHT, BLACK);
	gfx.draw_rect(g_tile_width / 2, gfx.SCREENHEIGHT - g_tile_height / 2, gfx.SCREENWIDTH - g_tile_width, g_tile_height / 2, BLACK);
	gfx.draw_rect(g_tile_width / 2, 0, gfx.SCREENWIDTH - g_tile_width, g_tile_height / 2, BLACK);

	if (mouse.LeftIsPressed())
	{
		int x = mouse.GetMouseX();
		int y = mouse.GetMouseY();

		int mouse_grid_x = floor((y / g_tile_height) + (x / g_tile_width) + g_tile_width / 2);
		int mouse_grid_y = floor((-x / g_tile_width) + (y / g_tile_height + g_tile_height / 2));
		
		//if (tile_x + floor(offX) >= 0 && tile_x + floor(offX) < g_tile_map.size() && tile_y + floor(offY) >= 0 && tile_y + floor(offY) < g_tile_map.size())
		//g_tile_map[mouse_grid_x + offX][mouse_grid_y + offY] = RED;
	}
	
	handle_user();
}

void Game::handle_user()
{
	double speed = 0.25;
	//if ((kbd.KeyIsPressed(VK_UP) && kbd.KeyIsPressed(VK_DOWN)) ||
	//	(kbd.KeyIsPressed(VK_UP) && kbd.KeyIsPressed(VK_LEFT)) ||
	//	(kbd.KeyIsPressed(VK_UP) && kbd.KeyIsPressed(VK_RIGHT)) ||
	//
	//	(kbd.KeyIsPressed(VK_DOWN) && kbd.KeyIsPressed(VK_LEFT)) ||
	//	(kbd.KeyIsPressed(VK_DOWN) && kbd.KeyIsPressed(VK_RIGHT)) ||
	//
	//	(kbd.KeyIsPressed(VK_LEFT) && kbd.KeyIsPressed(VK_RIGHT)))
	//	return;

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