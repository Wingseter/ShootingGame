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
	// 복사 생성자
	GameError(const GameError& e) :std::exception(e), errorCode(e.errorCode), message(e.message) {}
	// 기타 에러로 생성
	GameError(int code, const std::string &s) :errorCode(code), message(s) {}
	// = 오버로딩
	GameError& operator = (const GameError& rhs)
	{
		std::exception::operator=(rhs);
		this->errorCode = rhs.errorCode;
		this->message = rhs.message;
	}

	// 소멸자
	virtual ~GameError() {};
	
	const char * getMessage() const { return message.c_str(); }
	// 에러 코드 반환
	int getErrorCode() const { return errorCode; };
};
#endif