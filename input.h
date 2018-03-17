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
	bool keysDown[inputNS::KEYS_ARRAY_LEN];	// ���ǵ� Ű�� ����(down)���� true
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];	// ���ǵ� Ű�� (pressed)������ true
	std::string textIn;	// �Է��� text
	char charIn;	// ���������� �Է��� ����
	bool newLine;	// �ٹٲ� �Է�
	int mouseX, mouseY;	// ���콺 �Է�
	int mouseRawX, mouseRawY;	// ����� ���콺 �Է�
	int mouseWheel;	// ���콺 ��

	RAWINPUTDEVICE Rid[1];	// ����� ���콺���� ���
	bool mouseCaptured;	// ���콺�� ĸ�� �ǰ� ������ true
	bool mouseLButton;	// ���콺 ���ʹ�ư ������ true
	bool mouseMButton;	// ���콺 �� ������ true
	bool mouseRButton;	// ���콺 ������ ������ true
	bool mouseX1Button;	// ���콺 Ư��Ű1 ������ true
	bool mouseX2Button;	// ���콺 Ư��Ű2 ������ true
	ControllerState controllers[MAX_CONTROLLERS];	// ��Ʈ�ѷ� ����
	short thumbstickDeadzone;
	short triggerDeadzone;

public:
	// ������
	Input();
	
	// �Ҹ���
	virtual ~Input();

	// ���콺�� ��Ʈ�ѷ� �Է� ����
	void initialize(HWND hwnd, bool capture);

	// Ű ����
	void keyDown(WPARAM);

	// Ű �ö�
	void keyUp(WPARAM);

	// Ű �Է�
	void keyIn(WPARAM);

	// ���� Ű �Է� Ȯ��
	bool isKeyDown(UCHAR vKey) const;

	// Ű�� ����������
	bool wasKeyPressed(UCHAR vKey) const;

	// ���� �����ӿ��� � Ű���� ����������
	bool anyKeyPressed() const;
	
	// ������ Ű clear
	void clearKeyPress(UCHAR vKey);

	// ��� Ű���� ���� Ŭ����
	void clear(UCHAR what);

	// ��� Ű ����
	void clearAll() { clear(inputNS::KEYS_MOUSE_TEXT); }

	void clearTextIn() { textIn.clear(); }

	// �Է��� �ý�Ʈ ����
	void clearCharIn() { charIn = 0; }

	// �Է��� �ý�Ʈ ��ȯ
	std::string getTextIn() { return textIn; }

	void setTextIn(std::string str) { textIn = str; }

	// �ϰ��� ���� ��ȯ
	void getCharIn(LPARAM);

	// ���콺 �ø���
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

	// ���� ���콺�� ���� ������ ��ŭ ������
	int getMouseRawX()
	{
		int rawX = mouseRawX;
		mouseRawX = 0;
		return rawX;
	}

	// ���콺 ��
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

	// ��Ʈ�ѷ�  ���� ������Ʈ
	void checkControllers();

	// ��Ʈ�ѷ� �Է°� �˻�
	void readControllers();

	void setThumbstickDeadzone(short dz) { thumbstickDeadzone = abs(dz); }

	void setTriggerDeadzone(BYTE dz) { triggerDeadzone = dz; }

	short getThumbstickDeadzone() { return thumbstickDeadzone; }
	
	BYTE getTriggerDeadzone() { return static_cast<BYTE>(triggerDeadzone); }

	// ��Ʈ�ѷ��� ���¹�ȯ
	const ControllerState* getControllerState(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return &controllers[n];
	}

	// ��Ʈ�ѷ� ��ư
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

	// deadzone ���� ���� ����
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