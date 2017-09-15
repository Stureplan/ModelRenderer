#define WIN32_LEAN_AND_MEAN

//	MASS INCLUDES
#include <Windows.h>
#include "graphics.h"


//	VARIABLES

//	WEIRD WINDOWS SHIT
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//	MAIN LOOP
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	HWND window;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	RECT rect = { 0, 0, 1024, 768 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	window = CreateWindowEx(
		NULL,
		L"WindowClass",
		L"DX11 MAYA RENDERER",
		WS_OVERLAPPEDWINDOW,
		300, 300,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		hInstance,
		NULL);
	
	ShowWindow(window, iCmdshow);

	// setup DX11
	Graphics gfx;
	gfx.Initialize(window, SHADER_MODEL::AMBIENT_DIFFUSE_SPECULAR);

	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			gfx.Render();
		}
	}

	gfx.Unload();

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}