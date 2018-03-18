#include "game.h"

Game::Game()
{
	input = new Input();

	paused = false;
	graphics = NULL;
	initialized = false;
}

Game::~Game()
{
	deleteAll();
	ShowCursor(true);
}

LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (initialized)
	{
		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN: case WM_SYSKEYDOWN:
			input->keyDown(wParam);
			return 0;
		case WM_KEYUP: case WM_SYSKEYUP:
			input->keyUp(wParam);
			return 0;
		case WM_CHAR:
			input->keyIn(wParam);
			return 0;
		case WM_MOUSEMOVE:
			input->mouseIn(lParam);
			return 0;
		case WM_INPUT:			// 마우스 raw 입력
			input->mouseRawIn(lParam);
			return 0;
		case WM_LBUTTONDOWN:	//  왼쪽 버튼
			input->setMouseLButton(true);
			input->mouseIn(lParam);
			return 0;
		case WM_LBUTTONUP:		// 왼쪽 버튼
			input->setMouseLButton(false);
			input->mouseIn(lParam);
			return 0;
		case WM_MBUTTONDOWN:		// 중간 버튼
			input->setMouseMButton(true);
			input->mouseIn(lParam);
			return 0;
		case WM_MBUTTONUP:		// 중간버튼
			input->setMouseMButton(false);
			input->mouseIn(lParam);
			return 0;
		case WM_RBUTTONDOWN:
			input->setMouseRButton(true);
			input->mouseIn(lParam);
			return 0;
		case WM_RBUTTONUP:	
			input->setMouseRButton(false);
			input->mouseIn(lParam);
			return 0;
		case WM_XBUTTONDOWN: case WM_XBUTTONUP:
			input->setMouseXButton(wParam);
			input->mouseIn(lParam);
			return 0;
		case WM_MOUSEWHEEL:
			input->mouseWheelIn(wParam);
			return 0;
		case WM_DEVICECHANGE:
			input->checkControllers();
			return 0;
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Game::initialize(HWND hw)
{
	hwnd = hw;

	graphics = new Graphics();
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	input->initialize(hwnd, false);
	
	if(QueryPerformanceFrequency(&timeFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "error initializeing"));

	QueryPerformanceCounter(&timeStart);

	initialized = true;
}

void Game::renderGame()
{
	if (SUCCEEDED(graphics->beginScene()))
	{
		render();

		graphics->endScene();
	}

	handleLostGraphicsDevice();

	graphics->showBackBuffer();
}

void Game::handleLostGraphicsDevice()
{
	hr = graphics->getDeviceState();
	if (FAILED(hr))
	{
		if (hr == D3DERR_DEVICELOST)
		{
			Sleep(100);
			return;
		}
		else if (hr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			hr = graphics->reset();
			if (FAILED(hr))
				return;
			resetAll();
		}
		else
			return; // 다른 애러
	}
}

void Game::run(HWND hwnd)
{
	if (graphics == NULL)
		return;

	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) /
		(float)timeFreq.QuadPart;

	if (frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
		return;
	}

	if (frameTime > 0.0)
		fps = (fps * 0.99f) + (0.01f / frameTime);
	if (frameTime > MAX_FRAME_TIME)
		frameTime = MAX_FRAME_TIME;
	timeStart = timeEnd;

	if (!paused)
	{
		update();
		ai();
		collisions();
		input->vibrateControllers(frameTime);
	}
	renderGame();
	input->readControllers();

	input->clear(inputNS::KEYS_PRESSED);
}

void Game::releaseAll() {}

void Game::resetAll() {}

void Game::deleteAll()
{
	releaseAll();
	SAFE_DELETE(graphics);
	SAFE_DELETE(input);
	initialized = false;
}