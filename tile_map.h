#pragma once

#include <vector>
#include "D3DGraphics.h"

using namespace std;

class tile_map
{
public:
	tile_map(unsigned size);
	~tile_map(void);
	void draw(D3DGraphics & gfx);
	
private:
	void tile_to_screen_position(int tile_x, int tile_y, int & screen_x, int & screen_y, int tile_w, int tile_h);
	void screen_to_tile_position(int & tile_x, int & tile_y, int screen_x, int screen_y, int tile_w, int tile_h);
	vector<vector<D3DCOLOR>> map;
	int tile_h;
	int tile_w;

	int a1, a2, b1, b2, c1, c2, d1, d2;
};