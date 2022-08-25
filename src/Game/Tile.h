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

struct TileInfo
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

	bool operator==(const TileInfo& t) const;
};

constexpr uint16_t TileUnknown = 0;

struct Tile
{
	uint16_t tileInfoId = TileUnknown;
};

class TileTemplateManager
{
public:
	uint16_t AddTileTemplate(const TileInfo& tileTemplate);

	TileInfo& GetTileInfo(const Tile& tile);
private:
	std::vector<TileInfo> m_tiles;
};

extern TileTemplateManager gTileTemplateManager;