#pragma once

#ifndef _GAMEERROR_H
#define _GAMEERROR_H
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <exception>

class GameError;

namespace gameErrorNS
{
	const int FATAL_ERROR = -1;
	const int WARNING = 1;

}

class GameError : public std::exception
{
private:
	int errorCode;
	std::string message;
public:
	GameError() :errorCode(gameErrorNS::FATAL_ERROR), message("Undefined Error in game") {};
	// ���� ������
	GameError(const GameError& e) :std::exception(e), errorCode(e.errorCode), message(e.message) {}
	// ��Ÿ ������ ����
	GameError(int code, const std::string &s) :errorCode(code), message(s) {}
	// = �����ε�
	GameError& operator = (const GameError& rhs)
	{
		std::exception::operator=(rhs);
		this->errorCode = rhs.errorCode;
		this->message = rhs.message;
	}

	// �Ҹ���
	virtual ~GameError() {};
	
	const char * getMessage() const { return message.c_str(); }
	// ���� �ڵ� ��ȯ
	int getErrorCode() const { return errorCode; };
};
#endif