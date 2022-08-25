#include "stdafx.h"
#include "Tile.h"

TileTemplateManager gTileTemplateManager;

bool TileInfo::operator==(const TileInfo& a) const
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

uint16_t TileTemplateManager::AddTileTemplate(const TileInfo& tileTemplate)
{
	for (uint32_t i = 0; i < m_tiles.size(); i++)
	{
		if (tileTemplate == m_tiles[i])
			return static_cast<uint16_t>(i+1);
	}
	m_tiles.emplace_back(tileTemplate);
	return m_tiles.size();
}

TileInfo& TileTemplateManager::GetTileInfo(const Tile& tile)
{
	assert(tile.tileInfoId != TileUnknown);
	assert(tile.tileInfoId <= m_tiles.size());
	return m_tiles[tile.tileInfoId-1];
}