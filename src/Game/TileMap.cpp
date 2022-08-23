#include "stdafx.h"
#include "TileMap.h"
#include "TextureManager.h"
//-----------------------------------------------------------------------------
bool TileMap::Init()
{
	if (!m_tileGeometry.Init())
		return false;

	m_tiles = new TilesCell;


	// wall
	Texture2D* tex = gTextureManager->GetTexture2D("../data/textures/1.png");
	TileTemplate tileTemplate;
	tileTemplate.textureBack = tileTemplate.textureBottom = tileTemplate.textureForward = tileTemplate.textureLeft = tileTemplate.textureRight = tileTemplate.textureTop = tex;

	for (int x = 0; x < SizeMap; x++)
	{
		for (int y = 0; y < SizeMap; y++)
		{
			m_tiles->tiles[1][x][y] = nullptr;
			m_tiles->tiles[2][x][y] = nullptr;

			m_tiles->tiles[0][x][y] = new Tile();
			{
				m_tiles->tiles[0][x][y]->tileTemplate = gTileTemplateManager.AddTileTemplate(tileTemplate);
			}

			int r = rand() % 100;
			if (r < 10)
			{
				m_tiles->tiles[1][x][y] = new Tile();
				{
					m_tiles->tiles[1][x][y]->tileTemplate = gTileTemplateManager.AddTileTemplate(tileTemplate);
				}
			}

			if (r < 3)
			{
				m_tiles->tiles[2][x][y] = new Tile();
				{
					m_tiles->tiles[2][x][y]->tileTemplate = gTileTemplateManager.AddTileTemplate(tileTemplate);
				}
			}
		}
	}
	

	return true;
}
//-----------------------------------------------------------------------------
void TileMap::Close()
{
	for (int z = 0; z < SizeMapZ; z++)
	{
		for (int x = 0; x < SizeMap; x++)
		{
			for (int y = 0; y < SizeMap; y++)
			{
				delete m_tiles->tiles[z][x][y];
			}
		}
	}
	delete m_tiles;
	m_tileGeometry.Close();
}
//-----------------------------------------------------------------------------
void TileMap::Draw(const Camera& camera)
{
	m_tileGeometry.Draw(camera, m_tiles);
}
//-----------------------------------------------------------------------------