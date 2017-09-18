#define WIN32_LEAN_AND_MEAN

//	MASS INCLUDES
#include <Windows.h>
#include <shellapi.h>
#include <atlstr.h>

#include "graphics.h"

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

	RECT rect = { 0, 0, W, H };
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
	
	int amt;
	LPWSTR* str = CommandLineToArgvW(GetCommandLine(), &amt);
	std::string path = "export.fbx";

	if (amt > 1)
	{
		//path = CW2A(str[1]).m_psz;
	}

	std::string text = path;
	text.insert(0,"DX11 MAYA RENDERER: ");

	SetWindowTextA(window, text.c_str());

	// setup DX11
	Graphics gfx;
	gfx.Initialize(window, CW2A(str[0]).m_psz, path, SHADER_MODEL::AMBIENT_DIFFUSE_SPECULAR);

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