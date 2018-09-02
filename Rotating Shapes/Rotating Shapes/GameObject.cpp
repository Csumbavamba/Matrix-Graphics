#include "GameObject.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"



#include "Texture.h"
#include "Translateh.h"



GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::Initialise()
{

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(quad),
		quad,
		GL_STATIC_DRAW
	);

	// Binding Vertext attributes
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)(0)
	);

	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat))
	);

	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)(6 * sizeof(GLfloat))
	);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(indicies),
		indicies,
		GL_STATIC_DRAW
	);

	// SetupTexture();

	CalculateModelmatrix();
}

void GameObject::AddTexturePath(std::string texturePath)
{
	texture.filePath = texturePath;
}

void GameObject::SetupTexture()
{
	glGenTextures(1, &texture.texture);
	glBindTexture(GL_TEXTURE_2D, texture.texture);

	int width, height, channels;
	unsigned char* textureImage = SOIL_load_image(texture.filePath.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(textureImage);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GameObject::Render(GLuint program)
{
	glUseProgram(program);

	// Add texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.texture);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);

	// Draw Object
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void GameObject::Update()
{
	// Update position



	CalculateModelmatrix();
}

void GameObject::CalculateModelmatrix()
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(), translate.position);
	glm::mat4 rotationmatrix = glm::rotate(glm::mat4(), glm::radians(0.0f), translate.rotation); // TODO add rotation
	glm::mat4 scalingMatrix = glm::scale(glm::mat4(), translate.scale); // TODO add scaling

	// Model																				
	modelMatrix = translationMatrix * rotationmatrix * scalingMatrix;
}

glm::mat4 GameObject::GetModelMatrix() const
{
	return modelMatrix;
}

Texture GameObject::GetTexture() const
{
	return texture;
}
