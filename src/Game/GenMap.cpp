#include "stdafx.h"
#include "GenMap.h"
#include "Direction.h"

GenMap::GenMap(int width, int height)
	: width(width)
	, height(height)
	, tiles(width* height)
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
	tiles[x + y * width] = tile;
}

void GenMap::setTile(const Point& pos, GenTile tile)
{
	setTile(pos.x, pos.y, tile);
}

GenTile GenMap::getTile(int x, int y) const
{
	return tiles[x + y * width];
}

GenTile GenMap::getTile(const Point& pos) const
{
	return getTile(pos.x, pos.y);
}

void GenMap::fill(GenTile tile)
{
	std::fill(tiles.begin(), tiles.end(), tile);
}

void GenMap::move(std::vector<GenTile>&& tiles)
{
	this->tiles = std::move(tiles);
}

std::vector<GenTile> GenMap::copy()
{
	return tiles;
}