#include "graphics.h"

Graphics::Graphics()
{
	direct3d = NULL;
	device3d = NULL;
	fullscreen = false;
	width = GAME_WIDTH;
	height = GAME_HEIGHT;
	backColor = SETCOLOR_ARGB(255, 0, 0, 128); // dark blue
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

	// directX factory ����
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3d == NULL)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initialize Direct3D"));

	initD3Dpp();	// d3d �Ķ���� �־��ֱ�

	if (fullscreen)
	{
		if (isAdapterCompatible())
			d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		else
			throw(GameError(gameErrorNS::FATAL_ERROR,
				"The graphics device does not support the specified resolution"));

	}
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
	// �� ���� �����ֱ�
	result = device3d->Present(NULL, NULL, NULL, NULL);

	return result;
}

// ��� ȣȯ �˻�
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

// ��ġ lost �˻�
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
	safeRelease(device3d);
	safeRelease(direct3d);
}

// ����̽� ����
HRESULT Graphics::reset()
{
	initD3Dpp();
	result = device3d->Reset(&d3dpp);
	return result;
}
