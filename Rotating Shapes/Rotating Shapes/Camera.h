#pragma once

#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

class Camera
{
public:
	Camera();
	~Camera();

	glm::mat4 CreateViewMatrix();
	glm::mat4 CreatePerspectiveProjection();
	glm::mat4 CreateOrthographicProjection();

	void SetCameraPosition(glm::vec3 cameraPosition);
	void SetCameraLookDirection(glm::vec3 cameraLookDirection);
	void SetCameraUpDirection(glm::vec3 cameraUpDirection);

	glm::vec3 GetCameraPosition() const;
	glm::vec3 GetCameraLookDirection() const;
	glm::vec3 GetCameraUpDirection() const;

	void RotateAroundObject(glm::vec3 objectLocation, float distanceFromObject, float deltaTime);


private:
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraLookDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUpDirection = glm::vec3(0.0f, 1.0f, 0.0f);

	const unsigned int SCREEN_WIDTH = 800;
	const unsigned int SCREEN_HEIGHT = 800;
};

