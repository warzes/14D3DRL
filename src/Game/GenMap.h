#pragma once

#include "GenTile.h"

struct Point;

class GenMap
{
public:
	GenMap(int width, int height);

	bool isInBounds(int x, int y) const;
	bool isInBounds(const Point& pos) const;

	void setTile(int x, int y, GenTile tile);
	void setTile(const Point& pos, GenTile tile);

	GenTile getTile(int x, int y) const;
	GenTile getTile(const Point& pos) const;

	void fill(GenTile tile);

	// TODO: Better names?
	void move(std::vector<GenTile>&& tiles);
	std::vector<GenTile> copy();

public:
	const int width, height;

private:
	std::vector<GenTile> m_tiles;
};