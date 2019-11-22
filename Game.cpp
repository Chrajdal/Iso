#include "Game.h"
#include "Colors.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include "Timer.h"

static Timer timer;

Vei2 vWorldSize{ 555, 555 };
Vei2 vTileSize{ 40, 20 };
Vei2 vOrigin{ 12, -10 };

std::vector<std::vector<int>> pWorld (vWorldSize.y, std::vector<int>(vWorldSize.x, 0));
olc::Sprite* sprIsom = nullptr;

enum selection
{
	grass = 1, tree = 2, dead_tree = 3, sand = 4, water = 5
};

selection user_selection = grass;

Game::Game( HWND hWnd,KeyboardServer& kServer,const MouseServer& mServer )
:	gfx( hWnd ),
	audio( hWnd ),
	kbd( kServer ),
	mouse( mServer )
{
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

	auto elapsed = timer.elapsedns();
	std::cout << "FPS = " << 1e9 / elapsed << std::endl;
}

void Game::UpdateModel()
{
}

void Game::ComposeFrame()
{
	if (kbd.KeyIsPressed(VK_ESCAPE))
		exit(0);

	Vei2 vMouse = { mouse.GetMouseX(), mouse.GetMouseY() };

	// Work out active cell
	Vei2 vCell = { vMouse.x / vTileSize.x, vMouse.y / vTileSize.y };

	// Work out mouse offset into cell
	Vei2 vOffset = { vMouse.x % vTileSize.x, vMouse.y % vTileSize.y };

	// Sample into cell offset colour
	CColor col = sprIsom->GetColor(3 * vTileSize.x + vOffset.x, vOffset.y);

	// Work out selected cell by transforming screen cell
	Vei2 vSelected =
	{
		(vCell.y - vOrigin.y) + (vCell.x - vOrigin.x),
		(vCell.y - vOrigin.y) - (vCell.x - vOrigin.x)
	};

	// "Bodge" selected cell by sampling corners
	if (col == CColors::Red) vSelected += {-1, +0};
	if (col == CColors::Blue) vSelected += {+0, -1};
	if (col == CColors::Green) vSelected += {+0, +1};
	if (col == CColors::Yellow) vSelected += {+1, +0};

	// Handle mouse click to toggle if a tile is visible or not
	if (mouse.LeftIsPressed())
	{
		// Guard array boundary
		if (vSelected.x >= 0 && vSelected.x < vWorldSize.x && vSelected.y >= 0 && vSelected.y < vWorldSize.y)
			pWorld[vSelected.y][vSelected.x] = user_selection;
	}

	// Labmda function to convert "world" coordinate into screen space
	auto ToScreen = [&](int x, int y)
	{
		return Vei2
		{
			(vOrigin.x * vTileSize.x) + (x - y) * (vTileSize.x / 2),
			(vOrigin.y * vTileSize.y) + (x + y) * (vTileSize.y / 2)
		};
	};

	// Draw World - has binary transparancy so enable masking
	//SetPixelMode(olc::Pixel::MASK);
	gfx.m_mode = gfx.ALPHA;

	// (0,0) is at top, defined by vOrigin, so draw from top to bottom
	// to ensure tiles closest to camera are drawn last
	for (int y = 0; y < vWorldSize.y; y++)
	{
		for (int x = 0; x < vWorldSize.x; x++)
		{
			// Convert cell coordinate to world space
			Vei2 vWorld = ToScreen(x, y);
			if(	vWorld.x > -vTileSize.x &&
				vWorld.x < D3DGraphics::SCREENWIDTH + vTileSize.x &&
				vWorld.y > -vTileSize.y &&
				vWorld.y < D3DGraphics::SCREENHEIGHT + vTileSize.y)
			switch (pWorld[y][x])
			{
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

	// Draw Selected Cell - Has varying alpha components
	gfx.m_mode = gfx.ALPHA;

	// Convert selected cell coordinate to world space
	Vei2 vSelectedWorld = ToScreen(vSelected.x, vSelected.y);

	// Draw "highlight" tile
	gfx.DrawPartialSprite(vSelectedWorld.x, vSelectedWorld.y, sprIsom, 0 * vTileSize.x, 0, vTileSize.x, vTileSize.y);

	// Go back to normal drawing with no expected transparency
	gfx.m_mode = gfx.NORMAL;

	// Draw Hovered Cell Boundary
	//DrawRect(vCell.x * vTileSize.x, vCell.y * vTileSize.y, vTileSize.x, vTileSize.y, olc::RED);

	// Draw Debug Info
	gfx.DrawString(4, 4, "Mouse   : " + std::to_string(vMouse.x) + ", " + std::to_string(vMouse.y),        CColors::Black);
	gfx.DrawString(4, 14, "Cell    : " + std::to_string(vCell.x) + ", " + std::to_string(vCell.y),         CColors::Black);
	gfx.DrawString(4, 24, "Selected: " + std::to_string(vSelected.x) + ", " + std::to_string(vSelected.y), CColors::Black);

	handle_user();
}

void Game::handle_user()
{
	if (mouse.LeftIsPressed())
	{
	
	}
	if (mouse.RightIsPressed())
	{
	
	}

	if (kbd.KeyIsPressed(VK_UP))
	{
		vOrigin.y++;
	}
	if (kbd.KeyIsPressed(VK_DOWN))
	{
		vOrigin.y--;
	}
	if (kbd.KeyIsPressed(VK_LEFT))
	{
		vOrigin.x++;
	}
	if (kbd.KeyIsPressed(VK_RIGHT))
	{
		vOrigin.x--;
	}

	if (kbd.KeyIsPressed(0x31)) {
		user_selection = selection::grass;
	}
	if (kbd.KeyIsPressed(0x32)){
		user_selection = selection::tree;
	}
	if (kbd.KeyIsPressed(0x33)){
		user_selection = selection::dead_tree;
	}
	if (kbd.KeyIsPressed(0x34)){
		user_selection = selection::sand;
	}
	if (kbd.KeyIsPressed(0x35)){
		user_selection = selection::water;
	}
}