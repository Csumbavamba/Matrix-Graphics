#pragma once

#include <string>

#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include "Texture.h"
#include "Translateh.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Initialise();
	void AddTexturePath(std::string texturePath);
	void SetupTexture();

	void Render(GLuint program);
	void Update();
	void CalculateModelmatrix();

	glm::mat4 GetModelMatrix() const;

	// Translate is public
	Translate translate;
	Texture GetTexture() const;


private:

	
	glm::mat4 modelMatrix;
	Texture texture;

	GLuint VBO;
	GLuint VAO;
	GLuint EBO;
	static const int VERTICIES = 4;

	GLfloat quad[VERTICIES * 8] = // TODO Ask Callum wtf is wrong
	{
		// position				// color			// Tex Coords
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 1.0f,	0.0f, 0.0f,		// Top Left
		+0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 1.0f,	1.0f, 0.0f,		// Top Right
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 1.0f,	0.0f, 1.0f,		// Bottom Left
		+0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 1.0f,	1.0f, 1.0f		// Bottom Right
	};

	GLuint indicies[6] =
	{
		0, 2, 1,	// Top Left
		2, 3, 1		// Bottom Right
	};

};

