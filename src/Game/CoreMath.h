#pragma once

//=============================================================================
// Vector2
//=============================================================================

struct Vector2
{
public:
	Vector2() = default;
	Vector2(Vector2&&) = default;
	Vector2(const Vector2&) = default;
	Vector2(float n) : x(n), y(n) {}
	Vector2(float nx, float ny) : x(nx), y(ny) {}

	Vector2& operator=(Vector2&&) = default;
	Vector2& operator=(const Vector2&) = default;

	//bool operator==(const Vector2& a);
	//bool operator!=(const Vector2& a);
	
	Vector2 operator+(const Vector2& a) const { return { x + a.x, y + a.y }; }
	Vector2 operator-(const Vector2& a) const { return { x - a.x, y - a.y }; }
	Vector2 operator-() const { return { -x, -y }; }
	Vector2 operator*(const Vector2& a) const { return { x * a.x, y * a.y }; }
	Vector2 operator*(float n) const { return { x * n, y * n }; }
	Vector2 operator/(float n) const { return { x / n, y / n }; }
	Vector2& operator+=(const Vector2& a) { x += a.x; y += a.y; return(*this); }
	Vector2& operator-=(const Vector2& a) { x -= a.x; y -= a.y; return(*this); }
	Vector2& operator*=(float n) { x *= n; y *= n; return(*this); }
	Vector2& operator/=(float n) { x /= n; y /= n; return(*this); }

	//float& operator[](int Index) { return (*x)[Index]; }

	float GetLength() const;
	Vector2 Normalize() const;
	float DotProduct(const Vector2& a) const;

	float x = 0.0f;
	float y = 0.0f;
};

float Vector2::GetLength() const
{
	return sqrtf(x * x + y * y);
}

inline Vector2 Vector2::Normalize() const
{
	const float d = 1.0f / sqrt(x * x + y * y);
	return { x * d, y * d };
}

inline float Vector2::DotProduct(const Vector2& a) const
{
	return x * a.x + y * a.y;
}

//=============================================================================
// Vector3
//=============================================================================

struct Vector3
{
public:
	Vector3() = default;
	Vector3(Vector3&&) = default;
	Vector3(const Vector3&) = default;
	Vector3(float n) : x(n), y(n), z(n) {}
	Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

	Vector3& operator=(Vector3&&) = default;
	Vector3& operator=(const Vector3&) = default;

	//bool operator==(const Vector3& a);
	//bool operator!=(const Vector3& a);

	Vector3 operator+(const Vector3& a) const { return { x + a.x, y + a.y, z + a.z }; }
	Vector3 operator-(const Vector3& a) const { return { x - a.x, y - a.y, z - a.z }; }
	Vector3 operator-() const { return { -x, -y, -z }; }
	Vector3 operator*(const Vector3& a) const { return { x * a.x, y * a.y, z * a.z }; }
	Vector3 operator*(float n) const { return { x * n, y * n, z * n }; }
	Vector3 operator/(float n) const { return { x / n, y / n, z / n }; }
	Vector3& operator+=(const Vector3& a) { x += a.x; y += a.y; z += a.z; return(*this); }
	Vector3& operator-=(const Vector3& a) { x -= a.x; y -= a.y; z -= a.z; return(*this); }
	Vector3& operator*=(float n) { x *= n; y *= n; z *= n; return(*this); }
	Vector3& operator/=(float n) { x /= n; y /= n; z /= n; return(*this); }

	float GetLength() const;
	Vector3 Normalize() const;
	float DotProduct(const Vector3& a) const;
	Vector3 CrossProduct(const Vector3& vector) const;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

float Vector3::GetLength() const
{
	return sqrtf(x * x + y * y + z * z);
}

inline Vector3 Vector3::Normalize() const
{
	const float d = 1.0f / sqrt(x * x + y * y + z * z);
	return { x * d, y * d, z * d };
}

inline float Vector3::DotProduct(const Vector3& a) const
{
	return x * a.x + y * a.y + z * a.z;
}

inline Vector3 Vector3::CrossProduct(const Vector3& vector) const
{
	return { y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x };
}