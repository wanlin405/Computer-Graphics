#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
: Forward(glm::vec3(0.0f, 0.0f, -1.0f))
, MovementSpeed(SPEED)
, Mouse_Sensiticity(SENSITIVITY)
, Zoom(ZOOM)
{
    this->Position = position;
    this->World_up = up;
    this->Yaw = yaw;
    this->Pitch = pitch;
    UpdateCameraVectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch)
: Forward(glm::vec3(0.0f, 0.0f, -1.0f))
, MovementSpeed(SPEED)
, Mouse_Sensiticity(SENSITIVITY)
, Zoom(ZOOM)
{
	this->Position = glm::vec3(pos_x, pos_y, pos_z);
	this->World_up = glm::vec3(up_x, up_y, up_z);
	this->Yaw = yaw;
	this->Pitch = pitch;
	UpdateCameraVectors();
}

Camera::~Camera()
{

}

glm::mat4 Camera::GetViewMatrix()
{  
    return glm::lookAt(Position, Position + Forward, Up);
}

//对应键盘移动事件
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Forward * velocity;
	if (direction == BACKWARD)
		Position -= Forward * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
}
//对应鼠标移动事件
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= Mouse_Sensiticity;
	yoffset *= Mouse_Sensiticity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	UpdateCameraVectors();
}
//对应鼠标滚轮事件
void Camera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}


void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Forward = glm::normalize(front);
	Right = glm::normalize(glm::cross(Forward, World_up)); 
	Up = glm::normalize(glm::cross(Right, Forward));
}