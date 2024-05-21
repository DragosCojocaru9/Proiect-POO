#pragma once
#include "IUtilizator.h"
class Utilizator : public IUtilizator
{
	std::wstring username;
	std::wstring ID;
public:
	Utilizator() = default;
	Utilizator(const std::wstring user, const std::wstring id) :username(user), ID(id) {}

	std::wstring getUsername() { return username; }
	std::wstring getID() { return ID; }
	void setUsername(const std::wstring user) { username = user; }
	void setID(const std::wstring id) { ID = id; }
};