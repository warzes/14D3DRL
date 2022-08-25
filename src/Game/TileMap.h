#pragma once

/*
идея такая - карта разбита на ячейки, каждая ячейка содержит тайлы. Ячейки хранятся в файлах и выгружаются/загружаются по необходимости
*/

#include "TileMapGeometry.h"

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

private:
	TileMapGeometry m_tileGeometry;

	TilesCell* m_tiles = nullptr;
};