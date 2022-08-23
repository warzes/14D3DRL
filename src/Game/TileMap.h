#pragma once

#include "TileMapGeometry.h"

class TileMap
{
public:
	bool Init();
	void Close();

	void Draw(const Camera& camera);

private:
	TileMapGeometry m_tileGeometry;
};