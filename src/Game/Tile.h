#pragma once

class Texture2D;

enum class TileSide
{
	Top,
	Bottom,
	Forward,
	Back,
	Left,
	Right
};

enum class TileForm
{
	Air,
	Wall
};

struct TileTemplate
{
	Texture2D* textureTop = nullptr;
	Texture2D* textureBottom = nullptr;
	Texture2D* textureForward = nullptr;
	Texture2D* textureBack = nullptr;
	Texture2D* textureLeft = nullptr;
	Texture2D* textureRight = nullptr;
};

struct Tile
{
	TileTemplate* tileTemplate = nullptr;
	TileForm form = TileForm::Air; // тут чтобы можно было ломать стены
};