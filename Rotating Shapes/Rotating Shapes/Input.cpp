#include "Input.h"

#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"



void Input::KeyboardDown(unsigned char key, int x, int y)
{
	keyState[key] = DOWN;
}

void Input::KeyboardUp(unsigned char key, int x, int y)
{
	keyState[key] = UP;
}

