#pragma once

#include "glm/gtc/matrix_transform.hpp"

#define DEFAULT_YAW -90.0f
#define DEFAULT_PITCH 0.0f
#define DEFAULT_FOV 45.0f
#define DEFAULT_MOVEMENT_SPEED 2.5f
#define DEFAULT_MOUSE_SENSITIVITY 0.05f
#define DEFAULT_FLIGHT_ENABLED false

class Camera {
private:
    glm::vec3 m_Position, m_Front, m_Up, m_Right, m_WorldUp;
    float m_Yaw, m_Pitch, m_FOV;
    float m_MovementSpeed, m_MouseSensitivity;
    bool m_FlightEnabled;

    void UpdateCameraVectors();
public:
    enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

    Camera(glm::vec3 position, glm::vec3 up, float yaw=DEFAULT_YAW, float pitch=DEFAULT_PITCH, float fov=DEFAULT_FOV, bool flightEnabled=DEFAULT_FLIGHT_ENABLED);
    static glm::mat4 LookAt(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

    void ProcessKeyboard(int direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch=true);
    void ProcessMouseScroll(float yoffset);

    inline glm::mat4 GetViewMatrix() const { return LookAt(m_Position, m_Position + m_Front, m_Up); }

    inline float GetFOV() const { return m_FOV; }

    inline void SetFlightEnabled(bool flightEnabled) { m_FlightEnabled = flightEnabled; }
};