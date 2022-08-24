#include "stdafx.h"
#include "Tile.h"

TileTemplateManager gTileTemplateManager;

bool Tile::operator==(const Tile& a) const
{
	if (textureTop == a.textureTop &&
		textureBottom == a.textureBottom &&
		textureForward == a.textureForward &&
		textureBack == a.textureBack &&
		textureLeft == a.textureLeft &&
		textureRight == a.textureRight)
		return true;

	return false;
}

Tile* TileTemplateManager::AddTileTemplate(const Tile& tileTemplate)
{
	for (int i = 0; i < m_tiles.size(); i++)
	{
		if (tileTemplate == m_tiles[i]) 
			return &m_tiles[i];
	}

	m_tiles.push_back(tileTemplate);
	return &m_tiles[m_tiles.size()-1];
}