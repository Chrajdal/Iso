#include "Game.h"
#include "Colors.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include "Timer.h"

static Timer timer;

Vei2 vWorld{ 15, 10 };
Vei2 vTileSize{ 40, 20 };
Vei2 origin{ 5, 1 };

std::vector<std::vector<int>> world(vWorld.y, std::vector<int>(vWorld.x, 0));
olc::Sprite* sprIsom = nullptr;

Game::Game( HWND hWnd,KeyboardServer& kServer,const MouseServer& mServer )
:	gfx( hWnd ),
	audio( hWnd ),
	kbd( kServer ),
	mouse( mServer )
{
	srand((unsigned)time(0));


	for (int y = 0; y < vWorld.y; ++y) {
		for (int x = 0; x < vWorld.x; ++x) {
			world[y][x] = 0;
		}
	}

	// Load sprites used in demonstration
	sprIsom = new olc::Sprite("isometric_demo.png");
}

Game::~Game()
{
}

void Game::Go()
{
	timer.restart();
	UpdateModel();
	gfx.BeginFrame();
	ComposeFrame();
	gfx.EndFrame();

	auto elapsed = timer.elapsed();
	std::cout << "FPS = " << 1000.0 / elapsed << std::endl;
}

void Game::UpdateModel()
{
}

void Game::ComposeFrame()
{
	if (kbd.KeyIsPressed(VK_ESCAPE))
		exit(0);

	auto ToScreen = [&](int x, int y) {
		return Vei2
		{
			(origin.x * vTileSize.x) + (x - y) * vTileSize.x / 2,
			(origin.y * vTileSize.y) + (x + y) * vTileSize.y / 2
		};
	};

	Vei2 vMouse = { mouse.GetMouseX(), mouse.GetMouseY() };
	Vei2 vCell = { vMouse.x / vTileSize.x, vMouse.y / vTileSize.y };

	Vei2 vSelected =
	{
		(vCell.y - origin.y) + (vCell.x - origin.x),
		(vCell.y - origin.y) - (vCell.x - origin.x)
	};
	
	for (int y = 0; y < vWorld.y; ++y) {
		for (int x = 0; x < vWorld.x; ++x) {
			Vei2 vecWorld = ToScreen(x, y);

			switch (world[y][x]) {
			case 0:
				// Invisble Tile
				gfx.DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 1 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
				break;
			case 1:
				// Visible Tile
				gfx.DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 2 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
				break;
			case 2:
				// Tree
				gfx.DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 0 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y * 2);
				break;
			case 3:
				// Spooky Tree
				gfx.DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 1 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y * 2);
				break;
			case 4:
				// Beach
				gfx.DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 2 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y * 2);
				break;
			case 5:
				// Water
				gfx.DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 3 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y * 2);
				break;
			}
		}

	}

	// Convert selected cell coordinate to world space
	Vei2 vSelectedWorld = ToScreen(vSelected.x, vSelected.y);

	// Draw "highlight" tile
	gfx.DrawPartialSprite(vSelectedWorld.x, vSelectedWorld.y, sprIsom, 0 * vTileSize.x, 0, vTileSize.x, vTileSize.y);

	// Draw Hovered Cell Boundary
	gfx.draw_rect(vCell.x * vTileSize.x, vCell.y * vTileSize.y, vTileSize.x, vTileSize.y, CColors::Red.dword);

	gfx.DrawString(4, 4, "Mouse   : " + std::to_string(vMouse.x) + ", " + std::to_string(vMouse.y), CColors::Yellow);
	gfx.DrawString(4, 14, "Cell    : " + std::to_string(vCell.x) + ", " + std::to_string(vCell.y), CColors::Yellow);
	gfx.DrawString(4, 24, "Selected: " + std::to_string(vSelected.x) + ", " + std::to_string(vSelected.y), CColors::Yellow);

	handle_user();
}

void Game::handle_user()
{
	double speed = 0.25;

	if (mouse.LeftIsPressed())
	{
	
	
	}
	if (mouse.RightIsPressed())
	{
	
	}

	if (kbd.KeyIsPressed(VK_DOWN))
	{
	}
	if (kbd.KeyIsPressed(VK_UP))
	{
	}
	if (kbd.KeyIsPressed(VK_RIGHT))
	{
	}
	if (kbd.KeyIsPressed(VK_LEFT))
	{
	}
	if (kbd.KeyIsPressed(VK_SPACE))
	{
	}
}