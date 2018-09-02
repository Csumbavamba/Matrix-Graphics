#include "Camera.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"


Camera::Camera()
{
}


Camera::~Camera()
{
}

glm::mat4 Camera::CreateViewMatrix()
{
	return (glm::lookAt(cameraPosition, cameraLookDirection, cameraUpDirection));
}

glm::mat4 Camera::CreatePerspectiveProjection()
{
	return (glm::perspective(45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f));
}

glm::mat4 Camera::CreateOrthographicProjection()
{
	float halfScreenWidth = SCREEN_WIDTH / 2;
	float halfScreenHeight = SCREEN_HEIGHT / 2;

	return (glm::ortho(-halfScreenWidth, halfScreenWidth, -halfScreenHeight, halfScreenHeight, 0.1f, 100.0f));
}

void Camera::SetCameraPosition(glm::vec3 cameraPosition)
{
	this->cameraPosition = cameraPosition;
}

void Camera::SetCameraLookDirection(glm::vec3 cameraLookDirection)
{
	this->cameraLookDirection = cameraLookDirection;
}

void Camera::SetCameraUpDirection(glm::vec3 cameraUpDirection)
{
	this->cameraUpDirection = cameraUpDirection;
}

glm::vec3 Camera::GetCameraPosition() const
{
	return cameraPosition;
}

glm::vec3 Camera::GetCameraLookDirection() const
{
	return cameraLookDirection;
}

glm::vec3 Camera::GetCameraUpDirection() const
{
	return cameraUpDirection;
}

void Camera::RotateAroundObject(glm::vec3 objectLocation, float distanceFromObject, float deltaTime)
{
	float xMovement = sin(deltaTime) * distanceFromObject;
	float zMovement = cos(deltaTime) * distanceFromObject;

	cameraPosition = glm::vec3(objectLocation.x + xMovement, objectLocation.y, objectLocation.z + zMovement);
	cameraLookDirection = objectLocation;
}
