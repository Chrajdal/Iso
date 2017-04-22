#include "Game.h"
#include "Colors.h"
#include <cmath>
#include <ctime>
#include <vector>

using namespace std;

vector<vector<D3DCOLOR>> g_tile_map;
unsigned int g_size = 50;

unsigned int g_tile_height = 32;
unsigned int g_tile_width = g_tile_height * 2;

int a1, a2, b1, b2, c1, c2, d1, d2;

double offX = -(double(D3DGraphics::SCREENHEIGHT / 2) / (double)g_tile_height - double(D3DGraphics::SCREENWIDTH / 2) / (double)g_tile_width);
double offY = -(double(D3DGraphics::SCREENWIDTH / 2) / (double)g_tile_width + double(D3DGraphics::SCREENHEIGHT / 2) / (double)g_tile_height);

// yay using algebra outside of school!
void tile_to_screen_position(int tile_x, int tile_y, int & screen_x, int & screen_y, int tile_w, int tile_h)
{
	screen_x = (tile_x - tile_y) * tile_w / 2;
	screen_y = (tile_x + tile_y) * tile_h / 2;
}
void screen_to_tile_position(int & tile_x, int & tile_y, int screen_x, int screen_y, int tile_w, int tile_h)
{
	tile_x = int(double(screen_x) / (double)tile_w + double(screen_y) / (double)tile_h);
	tile_y = int(double(screen_y) / (double)tile_h - double(screen_x) / (double)tile_w);
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

	g_tile_map[g_size / 2][g_size / 2] = BLUE;

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

	//gfx.draw_tile(15 - offY, 5 - offX, g_tile_height, g_tile_width, RED);

	int startx = a1;
	int starty = b2;
	int endx = d1;
	int endy = c2;
	for (int i = startx; i <= endx; ++i)
	{
		for (int j = starty; j <= endy; ++j)
		{
			if (i + (int)offY >= 0 && i + (int)offY < g_tile_map.size() && j + (int)offX >= 0 && j + (int)offX < g_tile_map.size())
			{
				gfx.draw_tile(i, j, g_tile_height, g_tile_width, g_tile_map[j + (int)offX][i + (int)offY]);
			}
		}
	}

	//gfx.draw_rect(gfx.SCREENWIDTH / 2 - 10, gfx.SCREENHEIGHT / 2 - 25, 20, 30, BLUE);

	//gfx.draw_rect(0, 0, g_tile_width / 2, gfx.SCREENHEIGHT, BLACK);
	//gfx.draw_rect(gfx.SCREENWIDTH - g_tile_width / 2, 0, g_tile_width / 2, gfx.SCREENHEIGHT, BLACK);
	//gfx.draw_rect(g_tile_width / 2, gfx.SCREENHEIGHT - g_tile_height / 2, gfx.SCREENWIDTH - g_tile_width, g_tile_height / 2, BLACK);
	//gfx.draw_rect(g_tile_width / 2, 0, gfx.SCREENWIDTH - g_tile_width, g_tile_height / 2, BLACK);

	handle_user();
}

void Game::handle_user()
{
	double speed = 0.25;

	//if (mouse.LeftIsPressed())
	//{
	//	int x = 65; //mouse.GetMouseX();
	//	int y = 95; //mouse.GetMouseY();
	//	int mouse_grid_x;
	//	int mouse_grid_y;
	//	screen_to_tile_position(mouse_grid_x, mouse_grid_y, x, y, g_tile_width, g_tile_height);
	//
	//	if (mouse_grid_x >= 0 && mouse_grid_x < (int)g_tile_map.size() - 1 && mouse_grid_y >= 0 && mouse_grid_y < (int)g_tile_map.size() - 1)
	//		g_tile_map[mouse_grid_y][mouse_grid_x] = RED;
	//
	//}
	//if (mouse.RightIsPressed())
	//{
	//	int x = 65;
	//	int y = 95;
	//	int mouse_grid_x;
	//	int mouse_grid_y;
	//	screen_to_tile_position(mouse_grid_x, mouse_grid_y, x, y, g_tile_width, g_tile_height);
	//
	//	if (mouse_grid_x >= 0 && mouse_grid_x < (int)g_tile_map.size() - 1 && mouse_grid_y >= 0 && mouse_grid_y < (int)g_tile_map.size() - 1)
	//		g_tile_map[mouse_grid_y][mouse_grid_x] = D3DCOLOR_XRGB(rand() % 55, rand() % 200 + 55, rand() % 55);
	//}

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
		//g_tile_map.clear();
		unsigned size = g_tile_map.size() < 150 ? g_tile_map.size() * 2 : 150;
		vector < vector<D3DCOLOR>> tmpmap;
		for (unsigned int i = 0; i < size; ++i)
		{
			vector<D3DCOLOR> tmp;
			for (unsigned int j = 0; j < size; ++j)
			{
				tmp.push_back(D3DCOLOR_XRGB(rand() % 55, rand() % 200 + 55, rand() % 55));
			}
			tmpmap.push_back(tmp);
		}
		g_tile_map = tmpmap;
		g_tile_map[g_tile_map.size() / 2][g_tile_map.size() / 2] = RED;
	}
}