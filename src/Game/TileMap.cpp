#include "stdafx.h"
#include "TileMap.h"
//-----------------------------------------------------------------------------
bool TileMap::Init()
{
	if (!m_tileGeometry.Init())
		return false;

	m_tiles = new std::array<std::array<std::array<Tile, SizeMapZ>, SizeMap>, SizeMap>;
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
	m_tileGeometry.Draw(camera);
}
//-----------------------------------------------------------------------------