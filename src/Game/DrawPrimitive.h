#pragma once

class Camera;

namespace drawPrimitive
{
	void DrawLine(const Camera& camera, const glm::vec3& startPos, const glm::vec3& endPos);

	void DrawCubeWires(const Camera& camera, const glm::mat4& worldMatrix, const glm::vec4& color = { 1.0f, 0.0f, 0.0f, 1.0f }, bool disableDepthTest = false);
	void DrawCubeWires(const Camera& camera, const glm::vec3& position, const glm::vec3& size = glm::vec3(1.0f), const glm::vec3& rotationRadian = glm::vec3(0.0f), const glm::vec4& color = { 1.0f, 0.0f, 0.0f, 1.0f }, bool disableDepthTest = false);
	inline void DrawCubeWires(const Camera& camera, const glm::vec3& position, const glm::vec4& color, bool disableDepthTest = false)
	{
		drawPrimitive::DrawCubeWires(camera, position, glm::vec3(1.0f), glm::vec3(0.0f), color, disableDepthTest);
	}
	inline void DrawCubeWires(const Camera& camera, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, bool disableDepthTest = false)
	{
		drawPrimitive::DrawCubeWires(camera, position, size, glm::vec3(0.0f), color, disableDepthTest);
	}
}