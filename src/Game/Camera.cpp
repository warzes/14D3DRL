#include "stdafx.h"
#include "Camera.h"
#include "GameInput.h"
//-----------------------------------------------------------------------------
void Camera::MoveForward(float deltaTime, float speedMod)
{
	m_position += m_front * (m_movementSpeed * speedMod * deltaTime);
}
//-----------------------------------------------------------------------------
void Camera::MoveBackward(float deltaTime, float speedMod)
{
	m_position -= m_front * (m_movementSpeed * speedMod * deltaTime);
}
//-----------------------------------------------------------------------------
void Camera::MoveRight(float deltaTime, float speedMod)
{
	m_position += m_right * (m_movementSpeed * speedMod * deltaTime);
}
//-----------------------------------------------------------------------------
void Camera::MoveLeft(float deltaTime, float speedMod)
{
	m_position -= m_right * (m_movementSpeed * speedMod * deltaTime);
}
//-----------------------------------------------------------------------------
void Camera::MoveUp(float deltaTime, float speedMod)
{
	m_position += m_up * (m_movementSpeed * speedMod * deltaTime);
}
//-----------------------------------------------------------------------------
void Camera::MoveDown(float deltaTime, float speedMod)
{
	m_position -= m_up * (m_movementSpeed * speedMod * deltaTime);
}
//-----------------------------------------------------------------------------
void Camera::Rotate(float offsetX, float offsetY)
{
	m_yaw += offsetX * m_sensitivity;
	m_pitch += offsetY * m_sensitivity;

	if (m_pitch > 89.0f) m_pitch = 89.0f;
	else if (m_pitch < -89.0f) m_pitch = -89.0f;

	if (m_yaw > 360.0f) m_yaw = 0.0f;
	else if (m_yaw < -360.0f) m_yaw = 0.0f;

	updateVectors();
}
//-----------------------------------------------------------------------------
void Camera::SimpleMove(float deltaTime)
{
	//const float xpos = 1024 / 2;// gPlatformSystem->GetMouseX();
	//const float ypos = 768 / 2;// gPlatformSystem->GetMouseY();
	//static float lastPosX = xpos;
	//static float lastPosY = ypos;
	//Rotate((xpos - lastPosX), (lastPosY - ypos));
	//lastPosX = xpos;
	//lastPosY = ypos;

	constexpr float speedMod = 1.0f;
	if (IsKeyDown(Key::Up))
		MoveForward(deltaTime, speedMod);
	if (IsKeyDown(Key::Down))
		MoveBackward(deltaTime, speedMod);
	if (IsKeyDown(Key::StrafeRight))
		MoveRight(deltaTime, speedMod);
	if (IsKeyDown(Key::StrafeLeft))
		MoveLeft(deltaTime, speedMod);

	constexpr float speedRotateMod = 1600.0f;
	if (IsKeyDown(Key::Right))
		Rotate(speedRotateMod * deltaTime, 0.0f);
	if (IsKeyDown(Key::Left))
		Rotate(-speedRotateMod * deltaTime, 0.0f);

#ifdef _DEBUG
	if (IsKeyDown(Key::T))
		MoveUp(deltaTime, speedMod / 2.0f);
	if (IsKeyDown(Key::G))
		MoveDown(deltaTime, speedMod / 2.0f);

	if (IsKeyDown(Key::R))
		Rotate(0.0f, speedRotateMod * deltaTime);
	if (IsKeyDown(Key::F))
		Rotate(0.0f, -speedRotateMod * deltaTime);
#endif

	Update();
}
//-----------------------------------------------------------------------------
void Camera::Update()
{
	m_projectionMatrix = glm::perspective(glm::radians(m_fov), GetWindowAspect(), m_near, m_far);
	m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
}
//-----------------------------------------------------------------------------
void Camera::SetRotate(float yaw, float pitch)
{
	m_yaw = yaw;
	m_pitch = pitch;
	if (m_pitch > 89.0f) m_pitch = 89.0f;
	else if (m_pitch < -89.0f) m_pitch = -89.0f;
	if (m_yaw > 360.0f) m_yaw = 0.0f;
	else if (m_yaw < -360.0f) m_yaw = 0.0f;
	updateVectors();
}
//-----------------------------------------------------------------------------
Frustum Camera::ComputeFrustum() const
{
	Frustum frustum;

	const float halfVSide = m_far * tanf(m_fov * 0.5f);
	const float halfHSide = halfVSide * GetWindowAspect();
	const glm::vec3 frontMultFar = m_far * m_front;

	frustum.nearFace = { m_position + m_near * m_front, m_front };
	frustum.farFace = { m_position + frontMultFar, -m_front };
	frustum.rightFace = { m_position, glm::cross(m_up, frontMultFar + m_right * halfHSide) };
	frustum.leftFace = { m_position, glm::cross(frontMultFar - m_right * halfHSide, m_up) };
	frustum.topFace = { m_position, glm::cross(m_right, frontMultFar - m_up * halfVSide) };
	frustum.bottomFace = { m_position, glm::cross(frontMultFar + m_up * halfVSide, m_right) };

	return frustum;
}
//-----------------------------------------------------------------------------
void Camera::updateVectors()
{
	const glm::vec3 front = {
		cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
		sin(glm::radians(m_pitch)),
		sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
	};
	m_front = glm::normalize(front);
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
//-----------------------------------------------------------------------------