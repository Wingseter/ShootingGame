// perpose of this program is just for study
// this is a copy of the Books that called Programming 2D Games
// by Charles Kelly

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

// 함수 선언
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// 전역 변수
HINSTANCE hInst;
HDC hdc;
TCHAR ch = ' ';
RECT rect;
PAINTSTRUCT ps;

// 상수
const char CLASS_NAME[] = "Shooting";
const char APP_TITLE[] = "Shooting game";
const int WINDOW_WIDTAH = 400;
const int WINDOW_HEIGHT = 300;

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	MSG msg;

	// 윈도우 생성
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;

	int done = 0;
	// 메인 루프
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CHAR:
		switch (wParam)
		{
		case 0x08:	// 백스페이스
		case 0x09:	// 탭
		case 0x0A:	// 줄바꿈
		case 0x0D:	// 케리지 리턴
		case 0x1B:	// esc
			MessageBeep((UINT)-1);
			return 0;
		default:
			ch = (TCHAR)wParam;
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;
		}
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		TextOut(hdc, rect.right / 2, rect.bottom / 2, &ch, 1);
		EndPaint(hwnd, &ps);
		return 0;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = NULL;

	if (RegisterClassEx(&wcx) == 0)
		return false;

	hwnd = CreateWindow(
		CLASS_NAME,
		APP_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTAH,
		WINDOW_HEIGHT,
		(HWND)NULL,
		(HMENU)NULL,
		hInstance,
		(LPVOID)NULL);

	if (!hwnd)
		return false;

	ShowWindow(hwnd, nCmdShow);

	UpdateWindow(hwnd);
	return true;
}