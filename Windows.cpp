#include <Windows.h>
#include <wchar.h>
#include "D3DGraphics.h"
#include "Game.h"
#include "Mouse.h"

static KeyboardServer kServ;
static MouseServer mServ;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		// ************ KEYBOARD MESSAGES ************ //
	case WM_KEYDOWN:
		if (!(lParam & 0x40000000)) // no thank you on the autorepeat
		{
			kServ.OnKeyPressed(wParam);
		}
		break;
	case WM_KEYUP:
		kServ.OnKeyReleased(wParam);
		break;
	case WM_CHAR:
		kServ.OnChar(wParam);
		break;
		// ************ END KEYBOARD MESSAGES ************ //

		// ************ MOUSE MESSAGES ************ //
	case WM_MOUSEMOVE:
	{
		int x = (short)LOWORD(lParam);
		int y = (short)HIWORD(lParam);
		if (x > 0 && x < D3DGraphics::SCREENWIDTH && y > 0 && y < D3DGraphics::SCREENHEIGHT)
		{
			mServ.OnMouseMove(x, y);
			if (!mServ.IsInWindow())
			{
				SetCapture(hWnd);
				mServ.OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				x = max(0, x);
				x = min(D3DGraphics::SCREENWIDTH - 1, x);
				y = max(0, y);
				y = min(D3DGraphics::SCREENHEIGHT - 1, y);
				mServ.OnMouseMove(x, y);
			}
			else
			{
				ReleaseCapture();
				mServ.OnMouseLeave();
				mServ.OnLeftReleased();
				mServ.OnRightReleased();
			}
		}
	}
	break;
	case WM_LBUTTONDOWN:
		mServ.OnLeftPressed();
		break;
	case WM_RBUTTONDOWN:
		mServ.OnRightPressed();
		break;
	case WM_LBUTTONUP:
		mServ.OnLeftReleased();
		break;
	case WM_RBUTTONUP:
		mServ.OnRightReleased();
		break;
		// ************ END MOUSE MESSAGES ************ //
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX),CS_CLASSDC,MsgProc,0,0,
					  GetModuleHandle(NULL),NULL,NULL,NULL,NULL,
					  L"Iso Window",NULL };
	//wc.hIconSm = (HICON)LoadImage( hInst,MAKEINTRESOURCE( IDI_APPICON16 ),IMAGE_ICON,16,16,0 );
	//wc.hIcon   = (HICON)LoadImage( hInst,MAKEINTRESOURCE( IDI_APPICON32 ),IMAGE_ICON,32,32,0 );
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClassEx(&wc);

	RECT wr;
	wr.left = 150;
	wr.right = D3DGraphics::SCREENWIDTH + wr.left;
	wr.top = 50;
	wr.bottom = D3DGraphics::SCREENHEIGHT + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	HWND hWnd = CreateWindowW(L"Iso Window", L"Iso Framework",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, wr.left, wr.top, wr.right - wr.left, wr.bottom - wr.top,
		NULL, NULL, wc.hInstance, NULL);

	ShowWindow(hWnd, SW_MAXIMIZE ); //SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	Game theGame(hWnd, kServ, mServ);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//else
		{
			theGame.Go();
		}
	}

	UnregisterClass(L"Iso Window", wc.hInstance);
	return 0;
}