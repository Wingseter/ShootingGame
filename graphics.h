#pragma once
#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

class Graphics;

#include <d3d9.h>
#include <d3dx9.h>
#include "constants.h"
#include "gameError.h"

// directx ������ Ÿ��
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D		LPDIRECT3D9

// ���� ����
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
	((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

class Graphics
{
private:
	LP_3D direct3d;
	LP_3DDEVICE device3d;
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE pMode;

	HRESULT result;
	HWND hwnd;
	bool fullscreen;
	int width;
	int height;
	COLOR_ARGB backColor;
	
	// DirectX �ʱ�ȭ
	void initD3Dpp();

public:
	Graphics();

	virtual ~Graphics();

	// ����̽� ,��Ʈ���� ��ȯ
	void releaseAll();

	void initialize(HWND hw, int width, int height, bool fullscreen);

	// ȭ�� ����
	HRESULT showBackBuffer();

	bool isAdapterCompatible();

	// ����̽� �ʱ�ȭ
	HRESULT reset();

	// Getter and Setter
	LP_3D get3D() { return direct3d; }

	LP_3DDEVICE get3Ddevice() { return device3d; }

	HDC getDC() { return GetDC(hwnd); }

	HRESULT getDeviceState();

	void setBackColor(COLOR_ARGB c) { backColor = c; }

	HRESULT beginScene()
	{
		result = E_FAIL;
		if (device3d == NULL)
			return result;
		device3d->Clear(0, NULL, D3DCLEAR_TARGET, backColor, 1.0F, 0);
		result = device3d->BeginScene();
		return result;
	}

	HRESULT endScene()
	{
		result = E_FAIL;
		if (device3d)
			result = device3d->EndScene();
		return result;
	}
};

#endif