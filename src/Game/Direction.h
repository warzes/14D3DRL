#pragma once

struct Point
{
	Point() = default;
	Point(int nx, int ny) : x(nx), y(ny) {}

	bool operator==(const Point& p)
	{
		if (x == p.x && y == p.y) return true;
		else return false;
	}

	Point& operator+=(const Point& a) { x += a.x; y += a.y; return(*this); }
	Point& operator-=(const Point& a) { x -= a.x; y -= a.y; return(*this); }

	int x = 0;
	int y = 0;
};

class Direction : public Point
{
public:
	Direction() = default;
	Direction(int nx, int ny) : Point(nx,ny) {}

	Direction left45() const;
	Direction right45() const;

	Direction left90() const;
	Direction right90() const;

public:
	static const Direction None;
	static const Direction N;
	static const Direction NE;
	static const Direction E;
	static const Direction SE;
	static const Direction S;
	static const Direction SW;
	static const Direction W;
	static const Direction NW;

	static const std::array<Direction, 8> All;
	static const std::array<Direction, 4> Cardinal;
	static const std::array<Direction, 4> Diagonal;
};


inline Point operator-(const Point& d, const Point& p)
{
	return { d.x - p.x, d.y - p.y };
}
inline Point operator+(const Direction& d, const Point& p)
{
	return { d.x + p.x, d.y + p.y };
}
inline Point operator+(const Point& p, const Direction& d)
{
	return { d.x + p.x, d.y + p.y };
}
inline Direction operator*(const Direction& d, float f)
{
	return Direction( d.x * (int)f, d.y * (int)f );
}
inline Point operator/(const Point& p, float f)
{
	return Point{ p.x /(int)f, p.y / (int)f };
}


inline int length(const Point& vector)
{
	return static_cast<int>(std::sqrt(vector.x * vector.x + vector.y * vector.y));
}

inline int lengthSquared(const Point& vector)
{
	return vector.x * vector.x + vector.y * vector.y;
}

template <typename T>
inline int sign(T value)
{
	return (value > static_cast<T>(0)) - (value < static_cast<T>(0));
}

inline std::vector<Point> getLine(const Point& from, const Point& to, bool orthogonalSteps = false)
{
	Point delta = to - from;
	Point primaryIncrement(sign(delta.x), 0);
	Point secondaryIncrement(0, sign(delta.y));
	int primary = std::abs(delta.x);
	int secondary = std::abs(delta.y);

	if (secondary > primary)
	{
		std::swap(primary, secondary);
		std::swap(primaryIncrement, secondaryIncrement);
	}

	std::vector<Point> line;
	Point current = from;
	int error = 0;

	while (true)
	{
		line.emplace_back(current);

		if (current == to)
			break;

		current += primaryIncrement;
		error += secondary;

		if (error * 2 >= primary)
		{
			if (orthogonalSteps)
				line.emplace_back(current);

			current += secondaryIncrement;
			error -= primary;
		}
	}

	return line;
}