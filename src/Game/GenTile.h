#pragma once

enum class GenTile
{
	Unused,

	/* Dungeon */
	Floor,
	Corridor,
	Wall,
	ClosedDoor,
	OpenDoor,
	UpStairs,
	DownStairs,

	/* Forest */
	Grass,
	Tree,
	ClosedGate,
	OpenGate,
	Water,
	Bridge,

	/* Cave */
	Dirt,
	CaveWall,
	Lava,
	// Pit,

	/* Spaceship */
	Void,
	VoidWall,
};

struct Room
{
	Room() = default;
	Room(int rectLeft, int rectTop, int rectWidth, int rectHeight) :
		left(rectLeft),
		top(rectTop),
		width(rectWidth),
		height(rectHeight)
	{
	}

	bool intersects(const Room& rectangle) const;
	bool intersects(const Room& rectangle, Room& intersection) const;


	int left = 0;
	int top = 0;
	int width = 0;
	int height = 0;
};

inline bool Room::intersects(const Room& rectangle) const
{
	Room intersection;
	return intersects(rectangle, intersection);
}

inline bool Room::intersects(const Room& rectangle, Room& intersection) const
{
	// Rectangles with negative dimensions are allowed, so we must handle them correctly

	// Compute the min and max of the first rectangle on both axes
	int r1MinX = std::min(left, static_cast<int>(left + width));
	int r1MaxX = std::max(left, static_cast<int>(left + width));
	int r1MinY = std::min(top, static_cast<int>(top + height));
	int r1MaxY = std::max(top, static_cast<int>(top + height));

	// Compute the min and max of the second rectangle on both axes
	int r2MinX = std::min(rectangle.left, static_cast<int>(rectangle.left + rectangle.width));
	int r2MaxX = std::max(rectangle.left, static_cast<int>(rectangle.left + rectangle.width));
	int r2MinY = std::min(rectangle.top, static_cast<int>(rectangle.top + rectangle.height));
	int r2MaxY = std::max(rectangle.top, static_cast<int>(rectangle.top + rectangle.height));

	// Compute the intersection boundaries
	int interLeft = std::max(r1MinX, r2MinX);
	int interTop = std::max(r1MinY, r2MinY);
	int interRight = std::min(r1MaxX, r2MaxX);
	int interBottom = std::min(r1MaxY, r2MaxY);

	// If the intersection is valid (positive non zero area), then there is an intersection
	if ((interLeft < interRight) && (interTop < interBottom))
	{
		intersection = Room(interLeft, interTop, interRight - interLeft, interBottom - interTop);
		return true;
	}
	else
	{
		intersection = Room(0, 0, 0, 0);
		return false;
	}
}