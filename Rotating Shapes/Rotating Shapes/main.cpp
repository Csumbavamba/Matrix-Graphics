// Vertices and Textures
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

// Sound
#include "Dependencies\fmod\fmod.hpp"

// Matrix calculators
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

// Own classes
#include "ShaderLoader.h"
#include "GameObject.h"
#include "Camera.h"
#include "Input.h"

// Generic libraries
#include <vector>
#include <algorithm>

GLuint program;
GLfloat currentTime;

Camera mainCamera;
Input inputManager;
GameObject player, enemy;
std::vector<GameObject*> gameObjects;

// Sounds
FMOD::System * audioSystem = nullptr;
FMOD::Sound * fxThump;
FMOD::Sound * backgroundMusic;




// Essential Functions
void Render();
void Initialise();
void Update();

void UpdatePVM(GameObject * object);


// Sound
bool AudioInitialise();
bool CreateBackgroundMusic();
bool StartBackgroundMusic();
bool ReleaseSound();

// Input 
void ProcessInput();
void KeyboardDown(unsigned char key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);

void MouseClick(int button, int state, int x, int y);
void MousePassiveMove(int x, int y);
void MouseActiveMove(int x, int y);


int main (int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("Norton Defender");

	glClearColor(1.0, 0.0, 0.0, 1.0); // Clear Window

	glewInit();
	Initialise();

	// Register callbacks
	// StartBackgroundMusic();

	

	glutDisplayFunc(Render);
	glutIdleFunc(Update);
	
	// Input stuff
	glutKeyboardFunc(KeyboardDown);
	glutKeyboardUpFunc(KeyboardUp);
	//glutSpecialFunc();
	//glutSpecialUpFunc();
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseActiveMove);
	glutPassiveMotionFunc(MousePassiveMove);

	glutMainLoop();
	ReleaseSound();
	return 0;


}

void Initialise()
{
	ShaderLoader shaderLoader;
	program = shaderLoader.CreateProgram("VertexShader.vs", "FragmentShader.fs");

	AudioInitialise();
	CreateBackgroundMusic();

	player.Initialise();
	player.AddTexturePath("Sprites/AwesomeFace.png");
	player.SetupTexture();
	player.translate.scale *= 100.0f;

	enemy.Initialise();
	enemy.SetupTexture();
	enemy.translate.position.x = 100.0f;
	enemy.translate.scale *= 100.0f;

	gameObjects.push_back(&player);
	gameObjects.push_back(&enemy);


}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 0.0, 0.0, 1.0); // clear red

	// Render each gameObject
	for (GameObject * object : gameObjects)
	{
		object->Render(program);
		UpdatePVM(object);
	}


	
	GLuint currentTimeLocation = glGetUniformLocation(program, "currentTime");
	glUniform1f(currentTimeLocation, currentTime);

	// Update Transform
	

	glBindVertexArray(0); // Unbind VAO
	glUseProgram(0);


	glutSwapBuffers();
}

void Update()
{
	// Update game information
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	currentTime = currentTime / 1000.0f; // Now it's seconds instead of miliseconds

	// Update sound
	// audioSystem->update();
	// Check for input
	ProcessInput();
	for (GameObject * object : gameObjects)
	{
		object->Update();
	}

	glutPostRedisplay();
}


void UpdatePVM(GameObject * object)
{
	GLuint PVMLoc = glGetUniformLocation(program, "PVM");
	glm::mat4 PVM = mainCamera.CreateOrthographicProjection() * mainCamera.CreateViewMatrix() * object->GetModelMatrix();
	glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(PVM));
	
}

bool AudioInitialise()
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&audioSystem);

	if (result != FMOD_OK)
		return false;

	result = audioSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);

	if (result != FMOD_OK)
		return false;

	return true;
}

bool CreateBackgroundMusic()
{
	FMOD_RESULT result;
	result = audioSystem->createSound(
		"Audio/Background.mp3",
		FMOD_LOOP_NORMAL,
		0,
		&backgroundMusic
	);

	if (result != FMOD_OK)
		return false;
	return true;
}

bool StartBackgroundMusic()
{
	FMOD_RESULT result;
	result = audioSystem->playSound(backgroundMusic, 0, false, 0);

	if (result != FMOD_OK)
		return false;
	return true;
}

bool ReleaseSound()
{
	audioSystem->release();
	return true;
}

// Input functions

void ProcessInput()
{
	if (inputManager.keyState['w'] == DOWN)
	{
		player.translate.position.y += 0.25f;
	}
	if (inputManager.keyState['s'] == DOWN)
	{
		player.translate.position.y -= 0.25f;
	}
	if (inputManager.keyState['a'] == DOWN)
	{
		player.translate.position.x -= 0.25f;
	}
	if (inputManager.keyState['d'] == DOWN)
	{
		player.translate.position.x += 0.25f;
	}
}

void KeyboardDown(unsigned char key, int x, int y)
{
	inputManager.keyState[key] = DOWN;
}

void KeyboardUp(unsigned char key, int x, int y)
{
	inputManager.keyState[key] = UP;
}

void MouseClick(int button, int state, int x, int y)
{
	if (button >= 3)		// Avoid array getting out of bounds
		return;

	inputManager.mouseState[button] = (state == GLUT_DOWN) ? DOWN : UP;
}

void MousePassiveMove(int x, int y)
{
	std::cout << "Passive x: " << x << " | y: " << y << std::endl;
}

void MouseActiveMove(int x, int y)
{
	std::cout << "Clicked x: " << x << " | y: " << y << std::endl;
	// object1.position.x = x;
}