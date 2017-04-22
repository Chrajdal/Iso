#include "tile_map.h"

tile_map::tile_map(unsigned size)
{
	tile_h = 50;
	tile_w = 2 * tile_h;

	for (unsigned int i = 0; i < size; ++i)
	{
		vector<D3DCOLOR> tmp;
		for (unsigned int j = 0; j < size; ++j)
		{
			tmp.push_back(D3DCOLOR_XRGB(rand() % 55, rand() % 200 + 55, rand() % 55));
		}
		map.push_back(tmp);
	}

	map[size / 2][size / 2] = RED;

	screen_to_tile_position(a1, a2, 0, 0, tile_w, tile_h);
	screen_to_tile_position(b1, b2, D3DGraphics::SCREENWIDTH, 0, tile_w, tile_h);
	screen_to_tile_position(c1, c2, 0, D3DGraphics::SCREENHEIGHT, tile_w, tile_h);
	screen_to_tile_position(d1, d2, D3DGraphics::SCREENWIDTH, D3DGraphics::SCREENHEIGHT, tile_w, tile_h);
}

tile_map::~tile_map(void)
{

}

void tile_map::tile_to_screen_position(int tile_x, int tile_y, int & screen_x, int & screen_y, int tile_w, int tile_h)
{
	screen_x = (tile_x - tile_y) * tile_w / 2;
	screen_y = (tile_x + tile_y) * tile_h / 2;
}
void tile_map::screen_to_tile_position(int & tile_x, int & tile_y, int screen_x, int screen_y, int tile_w, int tile_h)
{
	tile_x = int(double(screen_x) / (double)tile_w + double(screen_y) / (double)tile_h);
	tile_y = int(double(screen_y) / (double)tile_h - double(screen_x) / (double)tile_w);
}

void tile_map::draw(D3DGraphics & gfx)
{
	int startx = a1;
	int starty = b2;
	int endx = d1;
	int endy = c2;
	for (int i = startx; i <= endx; ++i)
	{
		for (int j = starty; j <= endy; ++j)
		{
			//if (i + (int)offY >= 0 && i + (int)offY < g_tile_map.size() && j + (int)offX >= 0 && j + (int)offX < g_tile_map.size())
			//{
			//	gfx.draw_tile(i, j, g_tile_height, g_tile_width, g_tile_map[j + (int)offX][i + (int)offY]);
			//}
		}
	}
}