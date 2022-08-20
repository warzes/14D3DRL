#pragma once

struct Plane
{
	Plane() = default;
	Plane(const glm::vec3& p1, const glm::vec3& norm) : normal(glm::normalize(norm)), distance(glm::dot(normal, p1)) {}

	float GetSignedDistanceToPlan(const glm::vec3& point) const
	{
		return glm::dot(normal, point) - distance;
	}

	glm::vec3 normal = { 0.0f, 1.0f, 0.0f }; // unit vector
	float distance = 0.0f;                   // Distance with origin
};

struct Frustum
{
	Plane topFace;
	Plane bottomFace;

	Plane rightFace;
	Plane leftFace;

	Plane farFace;
	Plane nearFace;
};

class Camera
{
public:
	void MoveForward(float deltaTime, float speedMod = 1.0f);
	void MoveBackward(float deltaTime, float speedMod = 1.0f);
	void MoveRight(float deltaTime, float speedMod = 1.0f);
	void MoveLeft(float deltaTime, float speedMod = 1.0f);
	void Rotate(float offsetX, float offsetY);

	void SimpleMove(float deltaTime);
	void Update();

	void SetRotate(float yaw, float pitch);
	void SetPosition(const glm::vec3& pos) { m_position = pos; }
	void SetYaw(float val) { m_yaw = val; }
	void SetPitch(float val) { m_pitch = val; }
	void SetSpeed(float val) { m_movementSpeed = val; }
	void SetSensitivity(float val) { m_sensitivity = val; }
	void SetFov(float val) { m_fov = val; }
	void SetNear(float val) { m_near = val; }
	void SetFar(float val) { m_far = val; }

	float GetYaw() const { return m_yaw; }
	float GetPitch() const { return m_pitch; }

	const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
	const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
	const glm::vec3& GetPosition() const { return m_position; }
	const glm::vec3& GetDirection() const { return m_front; }

	Frustum ComputeFrustum() const;
	//private:
	void updateVectors();
private:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	// camera Attributes
	glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_right;
	glm::vec3 m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// euler Angles
	float m_yaw = -90.0f;
	float m_pitch = 0.0f;

	// camera options
	float m_movementSpeed = 7.0f;
	float m_sensitivity = 0.1f;
	float m_fov = 45.0f;
	float m_near = 0.01f;
	float m_far = 1000.0f;
};