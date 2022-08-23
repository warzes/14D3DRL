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

struct TileTemplate
{
	Texture2D* textureTop = nullptr;
	Texture2D* textureBottom = nullptr;
	Texture2D* textureForward = nullptr;
	Texture2D* textureBack = nullptr;
	Texture2D* textureLeft = nullptr;
	Texture2D* textureRight = nullptr;

	bool operator==(const TileTemplate& a) const;
};

class TileTemplateManager
{
public:
	TileTemplate* AddTileTemplate(const TileTemplate& tileTemplate);
private:
	std::vector<TileTemplate> m_tiles;
};

extern TileTemplateManager gTileTemplateManager;

struct Tile
{
	TileTemplate* tileTemplate = nullptr;
};