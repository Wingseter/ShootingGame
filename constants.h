// 상수
#pragma once
#ifndef _CONSTANTS_H
#define _CONSTANTS_H
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#define SAFE_DELETE(ptr) {if(ptr) {delete(ptr); (ptr) = NULL;}}
#define SAFE_RELEASE(ptr) {if(ptr) {(ptr)->Release(); (ptr)= NULL;}}
#define SAFE_DELETE_ARRAY(ptr) {if(ptr) {delete [](ptr); (ptr) = NULL;}}
#define SAFE_ON_LOST_DEVICE(ptr) {if(ptr) {ptr->onLostDevice();}}
#define SAFE_ON_RESET_DEVICE(ptr) {if(ptr) {ptr->onResetDevice();}}
#define TRANSCOLOR SETCOLOR_ARGB(0,255,0,255)

// 이미지
const char SKY_IMAGE[] = "picture\\sky.jpg";
const char CD_IMAGE[] = "picture\\cd.jpg";

const char CLASS_NAME[] = "Shooting";
const char GAME_TITLE[] = "Shooting game";
const bool FULLSCREEN = false;	// 전채 화면인지
const int GAME_WIDTH = 1280;	// 가로
const int GAME_HEIGHT = 840;	// 길이


// game
const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;
const float MIN_FRAME_RATE = 10.0f;
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;

// key
const UCHAR ESC_KEY = VK_ESCAPE;	// ESC 키 
const UCHAR ALT_KEY = VK_MENU;	// alt
const UCHAR ENTER_KEY = VK_RETURN;	// enter

// 인라인 함수
//template <typename T>
//inline void safeRelease(T& ptr)
//{
//	if (ptr)
//	{
//		ptr->Release();
//		ptr = NULL;
//	}
//}
//
//#define SAFE_RELEASE safeRelease
//
//template <typename T>
//inline void safeDelete(T& ptr)
//{
//	if (ptr)
//	{
//		delete ptr;
//		ptr = NULL;
//	}
//}
//#define SAFE_DELETE safeDelete
//
//template <typename T>
//inline void safeDeleteArray(T& ptr)
//{
//	if (ptr)
//	{
//		delete[] ptr;
//		ptr = NULL;
//	}
//}
//#define SAFE_DELETE_ARRAY safeDeleteArray
//
//template <typename T>
//inline void safeOnLoatDevice(T& ptr)
//{
//	if (ptr)
//		ptr->onLostDevice();
//}
//
//#define SAFE_ON_LOST_DEVICE safeOnLostDevice
//
//template <typename T>
//inline void safeOnResetDevice(T& ptr)
//{
//	if (ptr)
//		ptr->onResetDevice();
//}
//#define SAFE_ON_RESET_DEVICE safeOnResetDevice
#endif