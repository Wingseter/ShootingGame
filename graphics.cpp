#include "graphics.h"

Graphics::Graphics()
{
	direct3d = NULL;
	device3d = NULL;
	sprite = NULL;
	fullscreen = false;
	width = GAME_WIDTH;
	height = GAME_HEIGHT;
	backColor = graphicsNS::BACK_COLOR; // dark blue
}

Graphics::~Graphics()
{
	releaseAll();
}


void Graphics::initialize(HWND hw, int w, int h, bool full)
{
	hwnd = hw;
	width = w;
	height = h;
	fullscreen = full;

	// directX factory 생성
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3d == NULL)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initialize Direct3D"));

	initD3Dpp();	// d3d 파라미터 넣어주기

	if (fullscreen)
	{
		if (isAdapterCompatible())
			d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		else
			throw(GameError(gameErrorNS::FATAL_ERROR,
				"The graphics device does not support the specified resolution"));

	}
	// 장치 지원검사 필요있나?
	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING; // 소프트 웨어 프로세싱 사용
	else
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING; // 하드웨어 사용


	// 디바이스 생성
	result = direct3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		behavior,
		&d3dpp,
		&device3d);

	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error Creating Device"));
	
	result = D3DXCreateSprite(device3d, &sprite);
	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "ERROR Creating sprite"));
}

HRESULT Graphics::loadTexture(const char * filename, COLOR_ARGB transcolor, UINT & width, UINT & height, LP_TEXTURE & texture)
{
	D3DXIMAGE_INFO info;
	result = E_FAIL;

	try {
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}
		result = D3DXGetImageInfoFromFile(filename, &info);
		if (result != D3D_OK)
			return result;
		width = info.Width;
		height = info.Height;

		result = D3DXCreateTextureFromFileEx(
			device3d,
			filename,
			info.Width,
			info.Height,
			1,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,	
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			transcolor,
			&info,
			NULL,
			&texture);
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "ERROR in load texture"));
	}
	return result;
}

void Graphics::initD3Dpp()
{
	try {
		ZeroMemory(&d3dpp, sizeof(d3dpp));	// 귀찮으니 0으로 채워버리자
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		if (fullscreen)
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;	 // 24bit 색상
		else
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	// 기본 설정
		d3dpp.BackBufferCount = 1;	// 백버퍼 개수 설정
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // 화면 스왑 설정
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = (!fullscreen);
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing D3D presentation parameters"));
	}
}

HRESULT Graphics::showBackBuffer()
{
	// 백 버퍼 보여주기
	result = device3d->Present(NULL, NULL, NULL, NULL);

	return result;
}

// 기기 호환 검사
bool Graphics::isAdapterCompatible()
{
	UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT,
		d3dpp.BackBufferFormat);

	for (UINT i = 0; i < modes; i++)
	{
		result = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT,
			d3dpp.BackBufferFormat,
			i, &pMode);
		if (pMode.Height == d3dpp.BackBufferHeight &&
			pMode.Width == d3dpp.BackBufferWidth &&
			pMode.RefreshRate >= d3dpp.FullScreen_RefreshRateInHz)
			return true;
	}
	return false;
}

void Graphics::drawSprite(const SpriteData & spriteData, COLOR_ARGB color)
{
	if (spriteData.texture == NULL)
		return;

	// 중앙 찾기
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)(spriteData.width / 2 * spriteData.scale),
		(float)(spriteData.height / 2 * spriteData.scale));

	// 위치 잡기
	D3DXVECTOR2	 translate = D3DXVECTOR2((float)spriteData.x, (float)spriteData.y);

	// 크기 지정
	D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);

	if (spriteData.flipHorizontal)	// 좌우 대칭
	{
		scaling.x *= -1;

		spriteCenter.x -= (float)(spriteData.width*spriteData.scale);
		translate.x += (float)(spriteData.width*spriteData.scale);
	}

	if (spriteData.flipVertical)	// 상하 대장
	{
		scaling.y *= -1;

		spriteCenter.y -= (float)(spriteData.height*spriteData.scale);
		translate.y += (float)(spriteData.height*spriteData.scale);
	}

	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,
		NULL,
		0.0f,
		&scaling,
		&spriteCenter,
		(float)(spriteData.angle),
		&translate);
	sprite->SetTransform(&matrix);

	sprite->Draw(spriteData.texture, &spriteData.rect, NULL, NULL, color);
}

// 장치 lost 검사
HRESULT Graphics::getDeviceState()
{
	result = E_FAIL;
	if (device3d == NULL)
		return result;
	result = device3d->TestCooperativeLevel();
	return result;
}

// release
void Graphics::releaseAll()
{
	SAFE_RELEASE(sprite);
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}

// 디바이스 리셋
HRESULT Graphics::reset()
{
	initD3Dpp();
	result = device3d->Reset(&d3dpp);
	return result;
}

void Graphics::changeDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	try {
		switch (mode)
		{
		case graphicsNS::FULLSCREEN:
			if (fullscreen)
				return;
			fullscreen = true; break;
		case graphicsNS::WINDOW:
			if (fullscreen == false)
				return;
			fullscreen = false; break;
		default:
			fullscreen = !fullscreen;
		}
		reset();
		if (fullscreen)
		{
			SetWindowLong(hwnd, GWL_STYLE, WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP);
		}
		else
		{
			SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(hwnd, HWND_TOP, 0,0, GAME_WIDTH, GAME_HEIGHT,
				SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);
			MoveWindow(hwnd,
				0,
				0,
				GAME_WIDTH + (GAME_WIDTH - clientRect.right),
				GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom),
				TRUE);
		}
	}
	catch (...)
	{
		SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, GAME_WIDTH, GAME_HEIGHT,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

		RECT clientRect;
		GetClientRect(hwnd, &clientRect);
		MoveWindow(hwnd,
			0,
			0,
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom),
			TRUE);
	}
}
