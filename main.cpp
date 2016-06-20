#include "dxplayer.h"

#pragma comment(lib,"libmjpeg")

#define _CRT_SECURE_NO_WARNINGS
LRESULT CALLBACK WndProc(HWND hWnd,
	UINT nMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	static PAINTSTRUCT ps = { 0 };
	static HDC hdc;
	switch (nMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 1;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 1;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 1;
	default:
		break;
	}
	return DefWindowProc(hWnd, nMsg, wParam, lParam);
}

static bool registerClass(HINSTANCE hIns) {
	static bool isRegister = false;
	if (isRegister == true)
		return true;
	WNDCLASSEX wcx;

	// Fill in the window class structure with parameters 
	// that describe the main window. 

	wcx.cbSize = sizeof(wcx);          // size of structure 
	wcx.style = CS_HREDRAW |
		CS_VREDRAW;                    // redraw if size changes 
	wcx.lpfnWndProc = WndProc;     // points to window procedure 
	wcx.cbClsExtra = 0;                // no extra class memory 
	wcx.cbWndExtra = 0;                // no extra window memory 
	wcx.hInstance = hIns;         // handle to instance 
	wcx.hIcon = NULL;             // predefined app. icon 
	wcx.hCursor = LoadCursor(NULL,
		IDC_ARROW);                    // predefined arrow 
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);                  // white background brush 
	wcx.lpszMenuName = NULL;    // name of menu resource 
	wcx.lpszClassName = L"DXPLAYERWND";  // name of window class 
	wcx.hIconSm = NULL;

	// Register the window class. 
	if (RegisterClassEx(&wcx) != 0)
		isRegister = true;

	return isRegister;
}


HWND CreateWindowDefault(wchar_t* name, int x, int y, int width, int height, HINSTANCE hins) {
	HWND hwnd;

	if (!registerClass(hins))
		return false;

	hwnd = ::CreateWindowEx(0, L"DXPLAYERWND", name, WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX, x, y, width, height,
		NULL, NULL, hins, NULL);

	return hwnd;
}

HWND hWnd = NULL;

DWORD WINAPI err_handler(LPVOID v) {
	
	printf("error occur\n");
	PostMessage(hWnd, WM_QUIT, 0, 0);
	return 0;
}

int main() {

	HINSTANCE hins = GetModuleHandle(0);

	char* caption = "DxPlayer";

	DWORD_PTR dwResult;

	hWnd = CreateWindowDefault(L"DxPlayer", 20, 20, 640, 480, hins);

	if (hWnd == INVALID_HANDLE_VALUE)
	{
		return 1;
	}
	RECT rect;

	GetClientRect(hWnd, &rect);
	DxPlayer player(hWnd);

	if (player.initDriver(rect.right - rect.left, rect.bottom - rect.top) == false)
	{
		return false;
	}

	SendMessageTimeoutA(hWnd, WM_SETTEXT, 0,
		reinterpret_cast<LPARAM>(caption),
		SMTO_ABORTIFHUNG, 2000, &dwResult);

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	MSG msg = { 0 };

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		else {	
					
		}
		Sleep(1);
	}

	UnregisterClass(L"DXPLAYERWND",hins);
	//delete[] img.buffer;
	return 0;
}