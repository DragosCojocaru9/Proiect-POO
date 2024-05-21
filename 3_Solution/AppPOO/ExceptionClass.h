#pragma once
#include <iostream>
#include <string.h>
#include <vector>
class ExceptionClass
{
	std::string message;
public:
	ExceptionClass(const std::string& msg) :message(msg) {}
	const char* what() const {
		return message.c_str();
	}
};

