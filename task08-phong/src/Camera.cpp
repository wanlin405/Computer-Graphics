#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
: forward(glm::vec3(0.0f, 0.0f, -1.0f))
, mouse_speed(SPEED)
, mouse_sensiticity(SENSITIVITY)
, zoom(ZOOM)
{
    this->position = position;
    this->world_up = up;
    this->yaw = yaw;
    this->pitch = pitch;
    UpdateCameraVectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch)
{
    Camera(glm::vec3(pos_x, pos_y, pos_z), glm::vec3(up_x, up_y, up_z), yaw, pitch);
}

Camera::~Camera()
{

}

glm::mat4 Camera::GetViewMatrix()
{  
    return glm::lookAt(position, position + forward, up);
}

void Camera::Move(glm::vec3 distence)
{
    position += distence;
}

void Camera::Rotate(glm::vec3 rotate_vec3)
{
    yaw += rotate_vec3.x;
    pitch += (flip_y ? -1 : 1) * rotate_vec3.y;
    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    else if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }
    UpdateCameraVectors();
}

void Camera::Zoom(float zoom_offset)
{
    zoom += zoom_offset;
    if (zoom > 60.0f)
    {
        zoom = 60.0f;
    }
    else if (zoom < 1.0f)
    {
        zoom = 1.0f;
    }
}

void Camera::UpdateCameraVectors()
{
    float rad_yaw = glm::radians(yaw), rad_pitch = glm::radians(pitch);
    glm::vec3 tmp_forward(1);
    tmp_forward.x = cos(rad_yaw) * cos(rad_pitch);
    tmp_forward.y = sin(rad_pitch);
    tmp_forward.z = sin(rad_yaw) * cos(rad_pitch);
    forward = glm::normalize(tmp_forward);
    right = glm::normalize(glm::cross(tmp_forward, world_up));
    up = glm::normalize(glm::cross(right, tmp_forward));
}