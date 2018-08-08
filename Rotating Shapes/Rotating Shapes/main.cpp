#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "ShaderLoader.h"


#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

GLuint program;
GLuint quadVBO; // Vertex buffer Object
GLuint quadVAO; // vertex array object
GLuint quadEBO; // Element Buffer Object
GLfloat currentTime;

GLuint texture, texture2;
std::string raymanFilePath = "Sprites/Rayman.jpg";


// Matrix calculation demo
glm::vec3 objectiPosition = glm::vec3(0.5f, 0.5f, 0.0f);
glm::mat4 translationMatrix = glm::translate(glm::mat4(), objectiPosition);
glm::vec3 objectRotation = glm::vec3(0.0f, 0.0f, 1.0f);
float rotationAngle = 180;
glm::mat4 rotationZ = glm::rotate(glm::mat4(), glm::radians(rotationAngle), objectRotation);
glm::vec3 objectScale = glm::vec3(0.5f, 0.5f, 0.5f);
glm::mat4 scalingMatrix = glm::scale(glm::mat4(), objectScale);

glm::mat4 model = translationMatrix * rotationZ * scalingMatrix;

// Camera for prototyping
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraLookDirection = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUpDirection = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraLookDirection, cameraUpDirection);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 800;
glm::mat4 projectionMatrix = glm::perspective(45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

glm::mat4 PVM = projectionMatrix * viewMatrix * model;




GLfloat quad[] =
{
	// position				// color			// Tex Coords
	-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 1.0f,	0.0f, 0.0f,		// Top Left
	+0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 1.0f,	1.0f, 0.0f,		// Top Right
	-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 1.0f,	0.0f, 1.0f,		// Bottom Left
	+0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 1.0f,	1.0f, 1.0f		// Bottom Right
};

GLuint quadIndices[] =
{
	0, 2, 1,	// Top Left
	2, 3, 1		// Bottom Right
};

void Render();
void Initialise();
void Update();


int main (int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Rotating Shapes");

	glClearColor(1.0, 0.0, 0.0, 1.0); // Clear Window

	glewInit();
	Initialise();

	// Register callbacks
	glutDisplayFunc(Render);
	glutIdleFunc(Update);
	glutMainLoop();
	return 0;


}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 0.0, 0.0, 1.0); // clear red

	glUseProgram(program);

	// Add texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);

	// Draw quad with texture
	glBindVertexArray(quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	GLuint currentTimeLocation = glGetUniformLocation(program, "currentTime");
	glUniform1f(currentTimeLocation, currentTime);

	// Update Transform
	GLuint PVMLoc = glGetUniformLocation(program, "PVM");
	glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(PVM));

	glBindVertexArray(0); // Unbind VAO
	glUseProgram(0);


	glutSwapBuffers();
}

void Initialise()
{
	ShaderLoader shaderLoader;
	program = shaderLoader.CreateProgram("VertexShader.vs", "FragmentShader.fs");

	// @@@@ QUAD
	// Create quad VBO - VAO
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(quad),
		quad,
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,				// not normalized
		8 * sizeof(GLfloat),	// Stride
		(GLvoid*)0				// Offset
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

	// if (color.g == 1.0f && color.b == 1.0f && color.r == 0)

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// ADD EBO for quad
	glGenBuffers(1, &quadEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(quadIndices),
		quadIndices,
		GL_STATIC_DRAW
	);

	// Add textures
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, channels;
	unsigned char* raymanImage = SOIL_load_image(raymanFilePath.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, raymanImage);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(raymanImage);
	glBindTexture(GL_TEXTURE_2D, 0);

}


void Update()
{
	// Update game information
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	currentTime = currentTime / 1000.0f; // Now it's seconds instead of miliseconds

	



	glutPostRedisplay();
}

