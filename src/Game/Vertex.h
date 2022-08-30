#pragma once

#include "CoreMath.h"

struct Vertex_Pos2
{
	Vector2 position;
};

struct Vertex_Pos2_TexCoord
{
	Vector2 position;
	Vector2 texCoord;
};

struct Vertex_Pos3_Normal_TexCoord
{
	Vector3 position;
	Vector3 normal;
	Vector2 texCoord;
};