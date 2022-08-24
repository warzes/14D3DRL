#include "stdafx.h"
#include "TileMap.h"
#include "TextureManager.h"
//-----------------------------------------------------------------------------
bool TileMap::Init()
{
	if (!m_tileGeometry.Init())
		return false;

	m_tiles = new TilesCell;	
	for (int x = 0; x < SizeMap; x++)
	{
		for (int y = 0; y < SizeMap; y++)
		{
			Tile tileTemplate;
			// get texture
			{
				// wall
				Texture2D* tex = gTextureManager->GetTexture2D("../data/textures/1.png");

				int rt = rand() % 100;
				if (rt < 10)
					tex = gTextureManager->GetTexture2D("../data/textures/TILE_1A.png");
				else if (rt >= 10 && rt < 20)
					tex = gTextureManager->GetTexture2D("../data/textures/TILE_1B.png");
				else if (rt >= 20 && rt < 30)
					tex = gTextureManager->GetTexture2D("../data/textures/TILE_1C.png");
				else if (rt >= 30 && rt < 40)
					tex = gTextureManager->GetTexture2D("../data/textures/TILE_1D.png");
				else if (rt >= 40 && rt < 50)
					tex = gTextureManager->GetTexture2D("../data/textures/TILE_1E.png");
				else if (rt >= 50 && rt < 60)
					tex = gTextureManager->GetTexture2D("../data/textures/TILE_3A.png");
				else if (rt >= 60 && rt < 70)
					tex = gTextureManager->GetTexture2D("../data/textures/TILE_3B.png");
				else if (rt >= 70 && rt < 80)
					tex = gTextureManager->GetTexture2D("../data/textures/TILE_3C.png");
				else if (rt >= 80 && rt < 90)
					tex = gTextureManager->GetTexture2D("../data/textures/TILE_3D.png");
				else
					tex = gTextureManager->GetTexture2D("../data/textures/TILE_3E.png");
								
				tileTemplate.SetTexture(tex);
			}

			m_tiles->tiles[0][x][y] = gTileTemplateManager.AddTileTemplate(tileTemplate);
			m_tiles->tiles[1][x][y] = nullptr;
			m_tiles->tiles[2][x][y] = nullptr;
			m_tiles->tiles[3][x][y] = nullptr;
			m_tiles->tiles[4][x][y] = gTileTemplateManager.AddTileTemplate(tileTemplate);

			int r = rand() % 100;
			if (r < 10)
			{
				m_tiles->tiles[1][x][y] = gTileTemplateManager.AddTileTemplate(tileTemplate);
			}

			if (r < 3)
			{
				m_tiles->tiles[2][x][y] = gTileTemplateManager.AddTileTemplate(tileTemplate);
			}
		}
	}	

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