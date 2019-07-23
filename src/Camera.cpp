#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float fov, bool flightEnabled)
    : m_Position(position), m_Up(up), m_WorldUp(up), 
    m_Yaw(yaw), m_Pitch(pitch), m_FOV(fov), 
    m_MovementSpeed(CAMERA_DEFAULT_MOVEMENT_SPEED), m_MouseSensitivity(CAMERA_DEFAULT_MOUSE_SENSITIVITY), 
    m_FlightEnabled(flightEnabled)
{
    UpdateCameraVectors();
}

glm::mat4 Camera::LookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
    glm::mat4 translation(1.0f);
    translation[3][0] = -position.x;
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;

    glm::vec3 z_axis = glm::normalize(position - target);
    glm::vec3 x_axis = glm::normalize(glm::cross(glm::normalize(up), z_axis));
    glm::vec3 y_axis = glm::cross(z_axis, x_axis);

    glm::mat4 rotation(1.0f);
    rotation[0][0] = x_axis.x;
    rotation[1][0] = x_axis.y;
    rotation[2][0] = x_axis.z;
    rotation[0][1] = y_axis.x;
    rotation[1][1] = y_axis.y;
    rotation[2][1] = y_axis.z;
    rotation[0][2] = z_axis.x;
    rotation[1][2] = z_axis.y;
    rotation[2][2] = z_axis.z;

    return rotation * translation;
}

void Camera::ProcessKeyboard(int direction, float deltaTime) {
    float velocity = m_MovementSpeed * deltaTime;

    if (direction == FORWARD) {
        m_Position += m_Front * velocity;
    }
    if (direction == BACKWARD) {
        m_Position -= m_Front * velocity;
    }
    if (direction == LEFT) {
        m_Position -= m_Right * velocity;
    }
    if (direction == RIGHT) {
        m_Position += m_Right * velocity;
    }

	if (!m_FlightEnabled) {
	    m_Position.y = 0;
	}
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Yaw   += xoffset;
    m_Pitch += yoffset;

    if (constrainPitch) {
        if (m_Pitch >  89.0f) m_Pitch =  89.0f;
        if (m_Pitch < -89.0f) m_Pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    if (m_FOV >= 1.0f && m_FOV <= 45.0f) {
        m_FOV -= yoffset;
    }
    if (m_FOV <= 1.0f) {
        m_FOV = 1.0f;
    }
    if (m_FOV >= 45.0f) {
        m_FOV = 45.0f;
    }
}

void Camera::UpdateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);

    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up    = glm::normalize(glm::cross(m_Right, m_Front));
}