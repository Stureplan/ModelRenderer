#define WIN32_LEAN_AND_MEAN

//	MASS INCLUDES
#include <Windows.h>
#include <Windowsx.h>
#include <shellapi.h>
#include <atlstr.h>

#include "graphics.h"
#include "network.h"
#include "input.h"

//	FUNCTION PROTOTYPES
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void StartTime();
double Time();
double FrameTime();
double cPerSec = 0.0;
__int64 cStart = 0;
int frameCount = 0;
int fps = 0;
__int64 frameTimeOld = 0;
double frameTime;

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
	bool debug = false;

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
			frameCount++;
			if (Time() > 1.0f)
			{
				fps = frameCount;
				frameCount = 0;
				StartTime();
			}
			
			frameTime = FrameTime();

			std::string msg = "";
			if (net.Recieve(msg) > 0)
			{
				gfx.ParseMessage(msg);
			}


			if (input.IsKeyDown(VK_TAB))
			{
				debug = true;
			}
			else
			{
				debug = false;
			}

			if (input.IsKeyDown(VK_ESCAPE))
			{
				PostMessage(window, WM_QUIT, 0, 0);
			}

			gfx.Render(frameTime, debug);
		}
	}

	gfx.Unload();

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_KEYDOWN:
		{
			input.KeyDown((unsigned int)wParam);
			break;
		}
		case WM_KEYUP:
		{
			input.KeyUp((unsigned int)wParam);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);
			input.MousePosition(x, y);
			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void StartTime()
{
	LARGE_INTEGER freqCount;
	QueryPerformanceFrequency(&freqCount);
	cPerSec = double(freqCount.QuadPart);

	QueryPerformanceCounter(&freqCount);
	cStart = freqCount.QuadPart;
}

double Time()
{
	LARGE_INTEGER currTime;
	QueryPerformanceCounter(&currTime);
	return double(currTime.QuadPart - cStart) / cPerSec;
}

double FrameTime()
{
	LARGE_INTEGER cTime;
	__int64 tickCount;
	QueryPerformanceCounter(&cTime);

	tickCount = cTime.QuadPart - frameTimeOld;
	frameTimeOld = cTime.QuadPart;

	if (tickCount < 0.0f)
	{
		tickCount = 0.0f;
	}

	return float(tickCount) / cPerSec;
}