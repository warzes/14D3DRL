#pragma once

/*
идея такая - карта разбита на ячейки, каждая ячейка содержит тайлы. Ячейки хранятся в файлах и выгружаются/загружаются по необходимости
*/

#include "TileMapGeometry.h"

constexpr int SizeMap = 200;
constexpr int SizeMapZ = 5;


struct TilesCell
{
	TilesCell()
	{
		// в генераторе карт все обнуляется
		/*for (int z = 0; z < SizeMapZ; z++)
		{
			for (int x = 0; x < SizeMap; x++)
			{
				for (int y = 0; y < SizeMap; y++)
				{
					tiles[z][x][y] = nullptr;
				}
			}
		}*/
	}

	Tile* tiles[SizeMapZ][SizeMap][SizeMap];
};
constexpr int TileSize = sizeof(Tile);
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