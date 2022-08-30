#pragma once

#include "TileMapGeometry.h"
#include "GenMap.h"

constexpr int SizeMap = 70;
constexpr int SizeMapZ = 4;


struct TilesCell
{
	Tile tiles[SizeMapZ][SizeMap][SizeMap];
};
constexpr int TileSize = sizeof(TileInfo);
constexpr int TilesGridSize = sizeof(TilesCell) / 1024 / 1024;

struct TileDef
{
	bool isVisible = false;
	bool unused; // TODO: del
};

class TileMap
{
public:
	bool Init();
	void Close();

	void Draw(const Camera& camera);

	// x,y = player pos, z - rotate angle
	glm::vec3 GetStartPlayerPos() const;

	GenMap& GetMapTileData() { return m_map; }
	const GenMap& GetMapTileData() const { return m_map; }

	void SetTileVisible(int x, int y);
	bool GetTileVisible(int x, int y) const;

private:
	TileMapGeometry m_tileGeometry;
	GenMap m_map = GenMap(SizeMap, SizeMap);
	TilesCell* m_tiles = nullptr;

	std::vector<TileDef> m_tilesDef;
};