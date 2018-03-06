// perpose of this program is just for study
// this is a copy of the Books that called Programming 2D Games
// by Charles Kelly

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

// 함수 선언
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
bool AnotherInstance();
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// 전역 변수
HINSTANCE hInst;
HDC hdc;
TCHAR ch = ' ';
RECT rect;
PAINTSTRUCT ps;
bool vkKeys[256];

// 상수
const char CLASS_NAME[] = "Shooting";
const char APP_TITLE[] = "Shooting game";
const int WINDOW_WIDTAH = 400;
const int WINDOW_HEIGHT = 400;

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	MSG msg;

	// 중복실행 방지
	if (AnotherInstance())
		return false;

	// 윈도우 생성
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;

	for (int i = 0; i < 256; i++)
		vkKeys[i] = false;

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
	short nVirtKey;
	const short SHIFTED = (short)0x8000;
	TEXTMETRIC tm;
	DWORD chWidth = 20;
	DWORD chHeight = 20;

	switch (msg)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		ReleaseDC(hwnd, hdc);
		chWidth = tm.tmAveCharWidth;
		chHeight = tm.tmHeight;
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		vkKeys[wParam] = true;
		switch (wParam)
		{
		case VK_SHIFT:
			nVirtKey = GetKeyState(VK_LSHIFT);
			if (nVirtKey & SHIFTED)
				vkKeys[VK_LSHIFT] = true;
			nVirtKey = GetKeyState(VK_RSHIFT);
			if (nVirtKey & SHIFTED)
				vkKeys[VK_RSHIFT] = true;
			break;
		case VK_CONTROL:
			nVirtKey = GetKeyState(VK_LCONTROL);
			if (nVirtKey & SHIFTED)
				vkKeys[VK_LCONTROL] = true;
			nVirtKey = GetKeyState(VK_RCONTROL);
			if (nVirtKey & SHIFTED)
				vkKeys[VK_RCONTROL] = true;
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
		break;

	case WM_KEYUP:
		vkKeys[wParam] = false;
		switch (wParam)
		{
		case VK_SHIFT:
			nVirtKey = GetKeyState(VK_LSHIFT);
			if ((nVirtKey &SHIFTED) == 0)
				vkKeys[VK_RSHIFT] = false;
			nVirtKey = GetKeyState(VK_RSHIFT);
			if ((nVirtKey & SHIFTED) == 0)
				vkKeys[VK_RSHIFT] = false;
			break;
		case VK_CONTROL:
			nVirtKey = GetKeyState(VK_LCONTROL);
			if ((nVirtKey & SHIFTED) == 0)
				vkKeys[VK_LCONTROL] = false;
			nVirtKey = GetKeyState(VK_RCONTROL);
			if ((nVirtKey & SHIFTED) == 0)
				vkKeys[VK_RCONTROL] = false;
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 0x08:	// 백스페이스
		case 0x09:	// 탭
		case 0x0A:	// 줄바꿈
		case 0x0D:	// 케리지 리턴
		case 0x1B:	// esc
			return 0;
		default:
			ch = (TCHAR)wParam;
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;
		}
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 0, 0, &ch, 1);

		for (int r = 0; r < 16; r++)
		{
			for (int c = 0; c < 16; c++)
			{
				if (vkKeys[r * 16 + c])
				{
					SetBkMode(hdc, OPAQUE);
					TextOut(hdc, c * chWidth + chWidth * 2, r * chHeight + chHeight * 2, "T ", 2);
				}
				else {
					SetBkMode(hdc, TRANSPARENT);
					TextOut(hdc, c * chWidth + chWidth * 2, r * chHeight + chHeight * 2, "F ", 2);
				}
			}
		}
		EndPaint(hwnd, &ps);
		return 0;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

// 중복 실행 방지
bool AnotherInstance()
{
	HANDLE ourMutex;

	ourMutex = CreateMutex(NULL, true, "Use_a_different_string_here_for_each_program_48161 - XYZZY");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return true;
	return false;
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