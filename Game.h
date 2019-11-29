#pragma once

#include "D3DGraphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Sound.h"
#include "Timer.h"
#include "utils.h"

class Game
{
public:
	Game( HWND hWnd,KeyboardServer& kServer,const MouseServer& mServer );
	~Game();
	void Go();
private:
	void ComposeFrame(void);
	void handle_user();

private:
	D3DGraphics gfx;
	KeyboardClient kbd;
	MouseClient mouse;
	DSound audio;
	void UpdateModel();
	
public:
	enum class selection
	{
		grass = 1, tree = 2, dead_tree = 3, sand = 4, water = 5
	};
private:
	Vei2 vWorldSize;
	Vei2 vTileSize;
	Vei2 vOrigin;
	std::vector<std::vector<selection>> pWorld;
	olc::Sprite* sprIsom = nullptr;
	selection user_selection;


};