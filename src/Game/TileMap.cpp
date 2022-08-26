#include "stdafx.h"
#include "TileMap.h"
#include "TextureManager.h"
#include "Generator.h"
#include "GenDungeon.h"
#include "GenMap.h"
#include "Rng.h"
//-----------------------------------------------------------------------------
bool TileMap::Init()
{
	Generator* generator = new ClassicDungeon;
	Rng rng;

	generator->generate(m_map, rng);

	if (!m_tileGeometry.Init())
		return false;

	m_tiles = new TilesCell;	
	for (int x = 0; x < SizeMap; x++)
	{
		for (int y = 0; y < SizeMap; y++)
		{
			TileInfo tileTemplate;
			tileTemplate.SetTexture(
				gTextureManager->GetTexture2D("../data/textures/bricks.png"),
				gTextureManager->GetTexture2D("../data/textures/stonefloor.png"),
				gTextureManager->GetTexture2D("../data/textures/ceilingend.png")
			);

			switch (m_map.getTile(x, y))
			{
			case GenTile::Floor:
			case GenTile::Corridor:
			case GenTile::ClosedDoor:
			case GenTile::OpenDoor:
			case GenTile::UpStairs:
			case GenTile::DownStairs:
				m_tiles->tiles[0][x][y].tileInfoId = gTileTemplateManager.AddTileTemplate(tileTemplate);
				m_tiles->tiles[3][x][y].tileInfoId = gTileTemplateManager.AddTileTemplate(tileTemplate);
				break;

			case GenTile::Wall:
				m_tiles->tiles[1][x][y].tileInfoId = gTileTemplateManager.AddTileTemplate(tileTemplate);
				m_tiles->tiles[2][x][y].tileInfoId = gTileTemplateManager.AddTileTemplate(tileTemplate);
				break;
			default:
				break;
			}
		}
	}

	delete generator;

	return true;
}
//-----------------------------------------------------------------------------
void TileMap::Close()
{
	delete m_tiles;
	m_tileGeometry.Close();
}
//-----------------------------------------------------------------------------
void TileMap::Draw(const Camera& camera)
{
	m_tileGeometry.Draw(camera, m_tiles);
}
//-----------------------------------------------------------------------------
glm::vec3 TileMap::GetStartPlayerPos() const
{
	int numiter = 0;
	const int maxnumiter = 10000;
	while (1)
	{
		numiter++;
		if (numiter >= maxnumiter) break;

		int rx = rand() % SizeMap;
		int ry = rand() % SizeMap;

		if (m_map.getTile(rx, ry) == GenTile::Floor)
		{
			// TODO: доделать вращение
			// возможно возвращать не угол, а направление (left,right,back, forward), тогда будет легче проверить
			float rotate = 0.0f;

			int t1 = 0;
			int t2 = 0;
			int t3 = 0;
			int t4 = 0;

			for (int i = 0; i < 20; i++)
			{
				int leftPos = (rx - i);
				int rightPos = (rx + i);
				int forwardPos = (ry + i);
				int backPos = (ry - i);

				if (leftPos > 0 && m_map.getTile(leftPos, ry) == GenTile::Floor) t1++;
				if (rightPos < SizeMap-1 && m_map.getTile(rightPos, ry) == GenTile::Floor) t2++;
				if (backPos > 0 && m_map.getTile(rx, backPos) == GenTile::Floor) t3++;
				if (forwardPos < SizeMap - 1 && m_map.getTile(rx, forwardPos) == GenTile::Floor) t4++;
			}
			rotate = -90;
			if (t1 < t2) rotate = 90;
			if (t2 < t3) rotate = 0;
			if (t3 < t4) rotate = 180;

			return glm::vec3(rx, ry, rotate);
		}
			
	}

	return glm::vec3(0);
}
//-----------------------------------------------------------------------------