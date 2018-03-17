#pragma once
#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

class Graphics;

#include <d3d9.h>
#include "constants.h"
#include "gameError.h"

// directx 포인터 타입
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D		LPDIRECT3D9

// 색상 설정
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
	
	// DirectX 초기화
	void initD3Dpp();

public:
	Graphics();

	virtual ~Graphics();

	// 디바이스 ,디랙트엑스 반환
	void releaseAll();

	void initialize(HWND hw, int width, int height, bool fullscreen);

	// 화면 스왑
	HRESULT showBackBuffer();

	bool isAdapterCompatible();
};

#endif