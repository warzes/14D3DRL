#pragma once

#include "Camera.h"
#include "GameInput.h"
#include "CollisionMath.h"
#include "GenMap.h"

inline void PlayerRotateView(Camera& cam, float deltaTime)
{
	//const float xpos = 1024 / 2;// gPlatformSystem->GetMouseX();
	//const float ypos = 768 / 2;// gPlatformSystem->GetMouseY();
	//static float lastPosX = xpos;
	//static float lastPosY = ypos;
	//camera.Rotate((xpos - lastPosX), (lastPosY - ypos));
	//lastPosX = xpos;
	//lastPosY = ypos;
	constexpr float speedRotateMod = 1600.0f;
	if (IsKeyDown(Key::Right))
		cam.Rotate(speedRotateMod * deltaTime, 0.0f);
	if (IsKeyDown(Key::Left))
		cam.Rotate(-speedRotateMod * deltaTime, 0.0f);

#ifdef _DEBUG
	if (IsKeyDown(Key::R))
		cam.Rotate(0.0f, speedRotateMod * deltaTime);
	if (IsKeyDown(Key::F))
		cam.Rotate(0.0f, -speedRotateMod * deltaTime);
#endif
}

bool Player_TestCollision(const Vector3& newPlayerPos, const GenMap& map)
{
	BoundingBox pB = { {-0.3f, 0.0f, -0.3f}, {0.3f, 0.75f, 0.3f} };
	pB.min += newPlayerPos;
	pB.max += newPlayerPos;

	for (int x = (int)(newPlayerPos.x - 2); x < (int)(newPlayerPos.x + 2); x++)
	{
		if (x < 0 || x >= SizeMap) return true;
		{
			for (int y = (int)(newPlayerPos.z - 2); y < (int)(newPlayerPos.z + 2); y++)
			{
				if (y < 0 || y >= SizeMap) return true;

				if (map.getTile(x, y) == GenTile::Wall)
				{
					const float fx = static_cast<float>(x);
					const float fy = static_cast<float>(y);
					const BoundingBox blockB = {
						{-0.5f + fx, 0.0f, -0.5f + fy}, // min
						{ 0.5f + fx, 1.0f,  0.5f + fy}  // max
					};

					if (CheckCollisionBoxes(pB, blockB)) 
						return true;
				}
			}
		}
	}

	return false;
}

inline void FreeMove(Camera& cam, const GenMap& map, float deltaTime, bool isCollisionCheck)
{
	PlayerRotateView(cam, deltaTime);

	auto playerPos = cam.GetPosition();

	constexpr float speedMod = 1.0f;

	if (!isCollisionCheck)
	{
		if (IsKeyDown(Key::Up))
			cam.MoveForward(deltaTime, speedMod);
		if (IsKeyDown(Key::Down))
			cam.MoveBackward(deltaTime, speedMod);
		if (IsKeyDown(Key::StrafeRight))
			cam.MoveRight(deltaTime, speedMod);
		if (IsKeyDown(Key::StrafeLeft))
			cam.MoveLeft(deltaTime, speedMod);
#ifdef _DEBUG
		if (IsKeyDown(Key::T))
			cam.MoveUp(deltaTime, speedMod / 2.0f);
		if (IsKeyDown(Key::G))
			cam.MoveDown(deltaTime, speedMod / 2.0f);
#endif
	}
	else
	{
		glm::vec3 playerTempPos = cam.GetPosition();
		glm::vec3 playerOldPos = cam.GetPosition();
		glm::vec3 playerNewPos = cam.GetPosition();
		float movementSpeed = cam.GetSpeed();

		if (IsKeyDown(Key::Up))
		{
			playerTempPos += cam.GetDirection() * (movementSpeed * speedMod * deltaTime);
		}
		if (IsKeyDown(Key::Down))
		{
			playerTempPos -= cam.GetDirection() * (movementSpeed * speedMod * deltaTime);
		}
		if (IsKeyDown(Key::StrafeRight))
		{
			playerTempPos += cam.GetRight() * (movementSpeed * speedMod * deltaTime);
		}
		if (IsKeyDown(Key::StrafeLeft))
		{
			playerTempPos -= cam.GetRight() * (movementSpeed * speedMod * deltaTime);
		}

		// Move y. No gravitation
		playerNewPos.y = playerTempPos.y;

		//Move X & Test Collisions
		playerNewPos.x = playerTempPos.x;
		if (Player_TestCollision({ playerNewPos.x, playerNewPos.y, playerNewPos.z }, map))
		{
			playerNewPos.x = playerOldPos.x;
		}
		//Move Z & Test Collisions
		playerNewPos.z = playerTempPos.z;
		if (Player_TestCollision({ playerNewPos.x, playerNewPos.y, playerNewPos.z }, map))
		{
			playerNewPos.z = playerOldPos.z;
		}
		cam.SetPosition(playerNewPos);
	}
}