#include "Camera.h"

#include <iostream>

#include <GLFW/glfw3.h>

// MODULE

Camera::Camera()
	: m_FPSMode(true), m_FPSModeLast(true), m_SurvivalMode(false),
	m_camSpeed(0.05f),
	m_camPos(glm::vec3(0.0f, 0.0f, 5.0f)), m_camDirection(glm::vec3(0.0f, -1.0f, 0.0f)), m_camUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_camFront(glm::vec3(0.0f, 0.0f, -5.0f)), m_FOV(65.0f),
	m_yaw(-90.0f), m_pitch(0.0f),
	m_lastLook(glm::vec2(960 / 2, 540 / 2)), m_sensitivity(1.0f),
	m_x(1.0f), m_v(0.0f), m_goingUp(0), m_gravity(9.8f), m_jumping(false), m_delta(0.0f),
	m_zoom(2.0f), m_targetPos(glm::vec3(0.0f, 0.0f, 0.0f)), m_targetID(-1), m_spherical(true)
{
}

Camera::~Camera()
{
}

void Camera::IdleMovement(float delta)
{
	float camX = sin(delta) * m_camSpeed;
	float camZ = cos(delta) * m_camSpeed;

	m_camPos = glm::vec3(camX, 0.0f, camZ);
	m_camDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	m_camUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::Walk()
{
	auto& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard) {
		return;
	}
	if (ImGui::IsKeyDown(ImGuiKey_W))
	{
		glm::vec3 side = glm::normalize(glm::cross(m_camFront, m_camUp));
		m_camPos -= glm::normalize(glm::cross(side, glm::vec3(0.0f, 1.0f, 0.0f))) * m_camSpeed;
	}
	if (ImGui::IsKeyDown(ImGuiKey_S))
	{
		glm::vec3 side = glm::normalize(glm::cross(m_camFront, m_camUp));
		m_camPos += glm::normalize(glm::cross(side, glm::vec3(0.0f, 1.0f, 0.0f))) * m_camSpeed;
	}
	if (ImGui::IsKeyDown(ImGuiKey_A))
	{
		m_camPos -= glm::normalize(glm::cross(m_camFront, m_camUp)) * m_camSpeed;
	}
	if (ImGui::IsKeyDown(ImGuiKey_D))
	{
		m_camPos += glm::normalize(glm::cross(m_camFront, m_camUp)) * m_camSpeed;
	}
}

void Camera::Jump()
{
	if (ImGui::IsKeyPressed(ImGuiKey_Space) && m_camPos.y == 1.0f)
	{
		m_v = 6.0f;
		m_goingUp = 1;
		m_jumping = true;
	}
	if (m_jumping)
	{
		m_delta += 1 / 500.0f;
		if (m_goingUp)
		{
			m_v = m_v - m_gravity * m_delta;
			if (m_v <= 0.0f)
			{
				m_goingUp = 0;
				m_v = 0.0f;
			}
		}
		m_x = -(m_gravity / 2) * (m_delta * m_delta) + m_v * m_delta * m_goingUp + m_x;
		if (m_x <= 1.0f)
		{
			m_jumping = false;
			m_x = 1.0f;
			m_delta = 0.0f;
		}
	}

	m_camPos.y = m_x;
}

void Camera::Fly()
{
	auto& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard) {
		return;
	}
	if (ImGui::IsKeyDown(ImGuiKey_Space))
	{
		m_camPos += m_camSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
	}
	if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
	{
		m_camPos -= m_camSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
	}
}

void Camera::Look()
{
	auto& io = ImGui::GetIO();
	if (ImGui::IsMouseDown(0))
	{
		if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
			return;
		}

		float xpos = io.MousePos.x;
		float ypos = io.MousePos.y;

		float xoffset = xpos - m_lastLook.x;
		float yoffset = m_lastLook.y - ypos;
		m_lastLook.x = xpos;
		m_lastLook.y = ypos;

		xoffset *= m_sensitivity / 5;
		yoffset *= m_sensitivity / 5;

		m_yaw += xoffset;
		m_pitch += yoffset;

		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;

		m_camFront = glm::normalize(glm::vec3(
			cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
			sin(glm::radians(m_pitch)),
			sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
		));
	}
	else
		m_lastLook = glm::vec2(io.MousePos.x, io.MousePos.y);
}

void Camera::LookSphere()
{
	auto& io = ImGui::GetIO();
	if (ImGui::IsMouseDown(0))
	{
		if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
			return;
		}

		float xpos = io.MousePos.x;
		float ypos = io.MousePos.y;

		float xoffset = xpos - m_lastLook.x;
		float yoffset = m_lastLook.y - ypos;
		m_lastLook.x = xpos;
		m_lastLook.y = ypos;

		xoffset *= m_sensitivity / 2;
		yoffset *= m_sensitivity / 2;

		// Càmera Esfèrica
		m_yaw = m_yaw + xoffset;
		m_pitch = m_pitch + yoffset;

		if (m_pitch >= 360)		m_pitch = m_pitch - 360.0;
		if (m_pitch < 0)		m_pitch = m_pitch + 360.0;
		if (m_yaw >= 360)		m_yaw = m_yaw - 360.0;
		if (m_yaw < 0)			m_yaw = m_yaw + 360.0;
	}
	else
		m_lastLook = glm::vec2(io.MousePos.x, io.MousePos.y);

	m_camPos = glm::vec3(
		m_zoom * cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)) + m_targetPos.x,
		m_zoom * sin(glm::radians(m_pitch)) + m_targetPos.y,
		m_zoom * sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)) + m_targetPos.z
	);

	m_camUp = glm::vec3(
		-cos(glm::radians(m_yaw)) * sin(glm::radians(m_pitch)),
		cos(glm::radians(m_pitch)),
		-sin(glm::radians(m_yaw)) * sin(glm::radians(m_pitch))
	);
}

void Camera::SelectSpeed()
{
	if (ImGui::IsKeyReleased(ImGuiKey_DownArrow))
	{
		m_camSpeed -= 0.01f;
		if (m_camSpeed < 0.01f)
			m_camSpeed = 0.01f;
	}
	else if (ImGui::IsKeyReleased(ImGuiKey_UpArrow))
	{
		m_camSpeed += 0.01f;
		if (m_camSpeed > 1.0f)
			m_camSpeed = 1.0f;
	}
	else if (ImGui::IsKeyReleased(ImGuiKey_LeftArrow))
	{
		m_camSpeed -= 0.10f;
		if (m_camSpeed < 0.01f)
			m_camSpeed = 0.01f;
	}
	else if (ImGui::IsKeyReleased(ImGuiKey_RightArrow))
	{
		m_camSpeed += 0.10f;
		if (m_camSpeed > 1.0f)
			m_camSpeed = 1.0f;
	}
}

void Camera::ZoomSphere()
{
	if (ImGui::IsKeyDown(ImGuiKey_W))
	{
		m_zoom -= m_camSpeed;
		if (m_zoom < 0.0001f)
			m_zoom = 0.0001f;
	}
	if (ImGui::IsKeyDown(ImGuiKey_S))
	{
		m_zoom += m_camSpeed;
	}
}

void Camera::UpdateCam(float delta)
{
	if (m_FPSMode != m_FPSModeLast)
	{
		m_camPos = glm::vec3(0.0f, 0.0f, 3.0f);
		m_camDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		m_camUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_FPSModeLast = m_FPSMode;
	}

	if (!m_FPSMode)
	{
		IdleMovement(delta);
	}
	else
	{
		if (m_spherical)
		{
			SelectSpeed();
			LookSphere();
			ZoomSphere();
			m_camDirection = m_targetPos;
		}
		else
		{
			if (m_camUp != glm::vec3(0.0f, 1.0f, 0.0f)) {
				m_camUp = glm::vec3(0.0f, 1.0f, 0.0f);
				m_camFront = glm::normalize(-m_camPos);
				// Recover pitch and yaw:
				/*
				PITCH: we can recover it from the y coord of m_camFront
				YAW: we divide the circumference in 3 parts.
					- The first half of the circle yawSin (0,90) and yawCos (0,180) -> We use yawCos to recover YAW.
					- The second part is a quarter, where yawSin(-90,0) and yawCos(0,90) -> We use yawSin to recover YAW.
					- The third part is another quarter, where yawSin(-90,0) and yawCos(90,180) -> We use -yawCos to recover YAW.
				*/
				m_pitch = glm::degrees(asin(m_camFront.y));
				float yawSinRes = m_camFront.z / cos(glm::radians(m_pitch));
				float yawCosRes = m_camFront.x / cos(glm::radians(m_pitch));
				float yawSin = glm::degrees(asin(yawSinRes));
				float yawCos = glm::degrees(acos(yawCosRes));
				if (yawSin >= 0)
					m_yaw = yawCos;
				else if (yawCos <= 90)
					m_yaw = yawSin;
				else
					m_yaw = -yawCos;

			}
			SelectSpeed();
			Walk();
			if (m_SurvivalMode)
				Jump();
			else
				Fly();
			Look();
			m_camDirection = m_camPos + m_camFront;
		}
	}
}

void Camera::ResetView()
{
	m_camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_camFront = glm::normalize(-m_camPos / m_zoom);
	m_pitch = glm::degrees(asin(m_camFront.y));
	m_yaw = glm::degrees(acos(m_camFront.x / cos(glm::radians(m_pitch))));
}