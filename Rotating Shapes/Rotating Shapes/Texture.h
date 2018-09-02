#pragma once

#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include <string>

struct Texture
{
	std::string filePath = "Sprites/Rayman.jpg";
	GLuint texture = NULL;

};

