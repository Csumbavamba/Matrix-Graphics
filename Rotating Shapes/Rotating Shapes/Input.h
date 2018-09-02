#pragma once
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"


enum MouseState
{
	MOUSE_LEFT, // 0
	MOUSE_MIDDLE, // 1
	MOUSE_RIGHT // 2
};

enum InputState
{
	UP,
	DOWN,
	UP_FIRST,
	DOWN_FIRST
};



class Input
{
public:

	void KeyboardDown(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);

	InputState mouseState[3];
	InputState keyState[255];
};

