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

struct Tile
{
	void SetTexture(Texture2D* texture)
	{
		textureTop = textureBottom = textureForward = textureBack = textureLeft = textureRight = texture;
	}
	void SetTexture(Texture2D* side, Texture2D* top, Texture2D* bottom)
	{
		textureTop = top;
		textureBottom = bottom;
		textureForward = textureBack = textureLeft = textureRight = side;
	}

	Texture2D* textureTop = nullptr;
	Texture2D* textureBottom = nullptr;
	Texture2D* textureForward = nullptr;
	Texture2D* textureBack = nullptr;
	Texture2D* textureLeft = nullptr;
	Texture2D* textureRight = nullptr;

	bool operator==(const Tile& t) const;
};

class TileTemplateManager
{
public:
	Tile* AddTileTemplate(const Tile& tileTemplate);
private:
	std::vector<Tile> m_tiles;
};

extern TileTemplateManager gTileTemplateManager;