#include <Windows.h>
#pragma once
class Entity {
public:
	float X;
	float Y;
	float xSpeed;
	float ySpeed;
	bool jump;
	WCHAR symbol;
};