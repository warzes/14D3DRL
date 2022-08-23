#include "stdafx.h"
#include "TileMap.h"
//-----------------------------------------------------------------------------
bool TileMap::Init()
{
	if (!m_tileGeometry.Init())
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void TileMap::Close()
{
	m_tileGeometry.Close();
}
//-----------------------------------------------------------------------------
void TileMap::Draw(const Camera& camera)
{
	m_tileGeometry.Draw(camera);
}
//-----------------------------------------------------------------------------