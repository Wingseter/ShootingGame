#ifndef _INPUT_H
#define _INPUT_H
#define WIN32_LEAN_AND_MEAN

class Input;

#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <XInput.h>
#include "constants.h"
#include "gameError.h"

// high definition mouse
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((USHORT) 0x01)
#endif

#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((USHORT) 0x02)
#endif

namespace inputNS
{
	const int KEYS_ARRAY_LEN = 256;
	
	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR TEXT_IN = 8;
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}

const short GAMEPAD_THUMBSTICK_DEADZONE = (short)(0.20f * 0X7FFF);
const short GAMEPAD_TRIGGER_DEADZONE = 20;
const DWORD MAX_CONTROLLERS = 4;

const DWORD GAMEPAD_DPAD_UP = 0x0001;
const DWORD GAMEPAD_DPAD_DOWN = 0x0002;
const DWORD GAMEPAD_DPAD_LEFT = 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT = 0x0008;
const DWORD GAMEPAD_START_BUTTON = 0x0010;
const DWORD GAMEPAD_BACK_BUTTON = 0x0020;
const DWORD GAMEPAD_LEFT_THUMB = 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB = 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER = 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
const DWORD GAMEPAD_A = 0x1000;
const DWORD GAMEPAD_B = 0x2000;
const DWORD GAMEPAD_X = 0x4000;
const DWORD GAMEPAD_Y = 0x8000;

struct ControllerState
{
	XINPUT_STATE state;
	XINPUT_VIBRATION vibration;
	float vibrateTimeLeft;
	float vibrateTimeRight;
	bool connected;
};

class Input
{
private:
	bool keysDown[inputNS::KEYS_ARRAY_LEN];	// 정의된 키가 눌렀(down)을때 true
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];	// 정의된 키다 (pressed)됬을때 true
	std::string textIn;	// 입력한 text
	char charIn;	// 마지막으로 입력한 문자
	bool newLine;	// 줄바꿈 입력
	int mouseX, mouseY;	// 마우스 입력
	int mouseRawX, mouseRawY;	// 고오급 마우스 입력
	int mouseWheel;	// 마우스 휠

	RAWINPUTDEVICE Rid[1];	// 고오급 마우스에서 사용
	bool mouseCaptured;	// 마우스가 캡쳐 되고 있으면 true
	bool mouseLButton;	// 마우스 왼쪽버튼 누르면 true
	bool mouseMButton;	// 마우스 휠 누르면 true
	bool mouseRButton;	// 마우스 오른쪽 누르면 true
	bool mouseX1Button;	// 마우스 특수키1 누르면 true
	bool mouseX2Button;	// 마우스 특수키2 누르면 true
	ControllerState controllers[MAX_CONTROLLERS];	// 컨트롤러 상태
	short thumbstickDeadzone;
	short triggerDeadzone;

public:
	// 생성자
	Input();
	
	// 소멸자
	virtual ~Input();

	// 마우스와 컨트롤러 입력 시작
	void initialize(HWND hwnd, bool capture);

	// 키 누름
	void keyDown(WPARAM);

	// 키 올라감
	void keyUp(WPARAM);

	// 키 입력
	void keyIn(WPARAM);

	// 가상 키 입력 확인
	bool isKeyDown(UCHAR vKey) const;

	// 키가 눌려졌는지
	bool wasKeyPressed(UCHAR vKey) const;

	// 이전 프레임에서 어떤 키든지 눌려졌는지
	bool anyKeyPressed() const;
	
	// 눌러진 키 clear
	void clearKeyPress(UCHAR vKey);

	// 모든 키조합 버퍼 클리어
	void clear(UCHAR what);

	// 모든 키 삭제
	void clearAll() { clear(inputNS::KEYS_MOUSE_TEXT); }

	void clearTextIn() { textIn.clear(); }

	// 입력한 택스트 삭제
	void clearCharIn() { charIn = 0; }

	// 입력한 택스트 반환
	std::string getTextIn() { return textIn; }

	void setTextIn(std::string str) { textIn = str; }

	// 일겨한 문자 반환
	void getCharIn(LPARAM);

	// 마우스 시리즈
	void mouseIn(LPARAM);

	void mouseRawIn(LPARAM);

	void mouseWheelIn(WPARAM);

	void setMouseLButton(bool b) { mouseLButton = b; }

	void setMouseMButton(bool b) { mouseMButton = b; }

	void setMouseRButton(bool b) { mouseRButton = b; }

	void setMouseXButton(WPARAM wParam) { mouseX1Button = (wParam & MK_XBUTTON1) ? true : false; 
	mouseX2Button = (wParam &MK_XBUTTON2) ? true : false;	}

	int getMouseX() const { return mouseX; }
	int getMouseY() const { return mouseY; }

	// 이전 마우스로 부터 떨어진 만큼 움직임
	int getMouseRawX()
	{
		int rawX = mouseRawX;
		mouseRawX = 0;
		return rawX;
	}

	// 마우스 휠
	int getMouseWheel()
	{
		int wheel = mouseWheel;
		mouseWheel = 0;
		return wheel;
	}

	bool getMouseLButton() const { return mouseLButton; }
	bool getMouseMButton() const { return mouseMButton; }
	bool getMouseRButton() const { return mouseRButton; }
	bool getMouseX1Button() const { return mouseX1Button; }
	bool getMouseX2Button() const { return mouseX2Button; }

	// 컨트롤러  상태 업데이트
	void checkControllers();

	// 컨트롤러 입력값 검사
	void readControllers();

	void setThumbstickDeadzone(short dz) { thumbstickDeadzone = abs(dz); }

	void setTriggerDeadzone(BYTE dz) { triggerDeadzone = dz; }

	short getThumbstickDeadzone() { return thumbstickDeadzone; }
	
	BYTE getTriggerDeadzone() { return static_cast<BYTE>(triggerDeadzone); }

	// 컨트롤러의 상태반환
	const ControllerState* getControllerState(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return &controllers[n];
	}

	// 컨트롤러 버튼
	const WORD getGamepadButtons(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.wButtons;
	}

	bool getGamepadPadUp(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_UP) != 0);
	}

	bool getGamepadDPadDown(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_DOWN) != 0);
	}

	bool getGamepadDPadLeft(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_LEFT) != 0);
	}

	bool getGamepadDPadRight(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_RIGHT) != 0);
	}

	bool getGamepadStart(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_START_BUTTON) != 0);
	}

	bool getGamepadBack(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_BACK_BUTTON) != 0);
	}

	bool getGamepadLeftThumb(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_THUMB) != 0);
	}

	bool getGamepadRightThumb(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_THUMB) != 0);
	}

	bool getGamepadLeftShoulder(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_SHOULDER) != 0);
	}

	bool getGamepadRightShoulder(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_SHOULDER) != 0);
	}

	bool getGamepadA(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_A) != 0);
	}

	bool getGamepadB(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_B) != 0);
	}

	bool getGamepadX(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_X) != 0);
	}

	bool getGamepadY(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_Y) != 0);
	}

	BYTE getGamepadLeftTrigger(UINT n);

	// deadzone 설정 하지 않음
	BYTE getGamepadLeftTriggerUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.bRightTrigger;
	}

	//
	BYTE getGamepadThumbRightTrigger(UINT n);

	BYTE getGamepadRightTriggerUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.bRightTrigger;
	}

	//
	SHORT getGamepadThumbLX(UINT n);

	SHORT getGamepadThumbLXUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.sThumbLX;
	}

	//
	SHORT getGamepadThumbLY(UINT n);

	SHORT getGamepadThumbLYUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.sThumbLY;
	}

	//
	SHORT getGamepadThumbRX(UINT n);

	SHORT getGamepadThumbRXUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)   // if invalid controller number
			n = MAX_CONTROLLERS - 1;    // force valid
		return controllers[n].state.Gamepad.sThumbRX;
	}

	//
	SHORT getGamepadThumbRY(UINT n);

	SHORT getGamepadThumbRYUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.sThumbRY;
	}

	void gamePadVibrateLeft(UINT n, WORD speed, float sec)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		controllers[n].vibration.wLeftMotorSpeed = speed;
		controllers[n].vibrateTimeLeft = sec;
	}

	void gamePadVibrateRight(UINT n, WORD speed, float sec)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		controllers[n].vibration.wRightMotorSpeed = speed;
		controllers[n].vibrateTimeRight = sec;
	}
	void vibrateControllers(float frameTime);
};

#endif