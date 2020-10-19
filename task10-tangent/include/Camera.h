#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 world_up;

	float yaw;
	float pitch;

	float mouse_speed;
	float mouse_sensiticity;
	float zoom;
	bool flip_y = false;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch);
	~Camera();

	glm::mat4 GetViewMatrix();
	void Move(glm::vec3 distence);
	void Rotate(glm::vec3 rotate_vec3);
	void Zoom(float zoom_offset);

private:
	void UpdateCameraVectors();
};

#endif