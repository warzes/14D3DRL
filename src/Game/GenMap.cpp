#include "stdafx.h"
#include "GenMap.h"
#include "Direction.h"

GenMap::GenMap(int width, int height)
	: width(width)
	, height(height)
	, m_tiles(static_cast<size_t>(width * height))
{
}

bool GenMap::isInBounds(int x, int y) const
{
	return x >= 0 && x < width&& y >= 0 && y < height;
}

bool GenMap::isInBounds(const Point& pos) const
{
	return isInBounds(pos.x, pos.y);
}

void GenMap::setTile(int x, int y, GenTile tile)
{
	m_tiles[static_cast<size_t>(x + y * width)] = tile;
}

void GenMap::setTile(const Point& pos, GenTile tile)
{
	setTile(pos.x, pos.y, tile);
}

GenTile GenMap::getTile(int x, int y) const
{
	return m_tiles[static_cast<size_t>(x + y * width)];
}

GenTile GenMap::getTile(const Point& pos) const
{
	return getTile(pos.x, pos.y);
}

void GenMap::fill(GenTile tile)
{
	std::fill(m_tiles.begin(), m_tiles.end(), tile);
}

void GenMap::move(std::vector<GenTile>&& tiles)
{
	m_tiles = std::move(tiles);
}

std::vector<GenTile> GenMap::copy()
{
	return m_tiles;
}