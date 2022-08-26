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

class TileMap
{
public:
	bool Init();
	void Close();

	void Draw(const Camera& camera);

	// x,y = player pos, z - rotate angle
	glm::vec3 GetStartPlayerPos() const;

	GenMap& GetMapTileData() { return m_map; }

private:
	TileMapGeometry m_tileGeometry;
	GenMap m_map = GenMap(SizeMap, SizeMap);
	TilesCell* m_tiles = nullptr;
};