#include "graphics.h"
#include "constants.h"

Graphics::Graphics()
{
	direct3d = NULL;
	device3d = NULL;
	fullscreen = false;
	width = GAME_WIDTH;
	height = GAME_HEIGHT;
}

Graphics::~Graphics()
{
	releaseAll();
}

void Graphics::releaseAll()
{
	safeRelease(device3d);
	safeRelease(direct3d);
}

void Graphics::initialize(HWND hw, int w, int h, bool full)
{
	hwnd = hw;
	width = w;
	height = h;
	fullscreen = full;

	// directX factory ����
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3d == NULL)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initialize Direct3D"));

	initD3Dpp();	// d3d �Ķ���� �־��ֱ�

	// ��ġ �����˻� �ʿ��ֳ�?
	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING; // ����Ʈ ���� ���μ��� ���
	else
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING; // �ϵ���� ���


	// ����̽� ����
	result = direct3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		behavior,
		&d3dpp,
		&device3d);

	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error Creating Device"));
}

void Graphics::initD3Dpp()
{
	try {
		ZeroMemory(&d3dpp, sizeof(d3dpp));	// �������� 0���� ä��������
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		if (fullscreen)
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;	 // 24bit ����
		else
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	// �⺻ ����
		d3dpp.BackBufferCount = 1;	// ����� ���� ����
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // ȭ�� ���� ����
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
	result = E_FAIL;
	// ����� �ʱ�ȭ
	device3d->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 0.0f, 0);

	// �� ���� �����ֱ�
	result = device3d->Present(NULL, NULL, NULL, NULL);

	return result;
}