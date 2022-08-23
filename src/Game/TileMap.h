#pragma once

#include "TileMapGeometry.h"

constexpr int SizeMap = 10000;
constexpr int SizeMapZ = 3;

class TileMap
{
public:
	bool Init();
	void Close();

	void Draw(const Camera& camera);

private:
	TileMapGeometry m_tileGeometry;

	std::array<std::array<std::array<Tile, SizeMapZ>, SizeMap>, SizeMap>* m_tiles;
};