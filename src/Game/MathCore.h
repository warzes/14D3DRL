#pragma once

//=============================================================================
// Vector2
//=============================================================================

struct Vector2
{
public:
	constexpr Vector2() = default;
	constexpr Vector2(Vector2&&) = default;
	constexpr Vector2(const Vector2&) = default;
	constexpr Vector2(float n) : x(n), y(n) {}
	constexpr Vector2(float nx, float ny) : x(nx), y(ny) {}

	constexpr Vector2& operator=(Vector2&&) = default;
	constexpr Vector2& operator=(const Vector2&) = default;

	//bool operator==(const Vector2& v);
	//bool operator!=(const Vector2& v);
	
	Vector2 operator+(float f) const { return { x + f, y + f }; }
	Vector2 operator+(const Vector2& v) const { return { x + v.x, y + v.y }; }
	Vector2 operator-() const { return { -x, -y }; }
	Vector2 operator-(float f) const { return { x - f, y - f }; }
	Vector2 operator-(const Vector2& v) const { return { x - v.x, y - v.y }; }
	Vector2 operator*(float f) const { return { x * f, y * f }; }
	Vector2 operator*(const Vector2& v) const { return { x * v.x, y * v.y }; }
	Vector2 operator/(float f) const { return { x / f, y / f }; }
	Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return(*this); }
	Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return(*this); }
	Vector2& operator*=(float f) { x *= f; y *= f; return(*this); }
	Vector2& operator/=(float f) { x /= f; y /= f; return(*this); }

	float* operator&() { return (float*)this; };
	operator float*() { return &x; }
	operator const float*() const { return &x; }

	float operator[](size_t i) { return (&x)[i]; }
	const float operator[](size_t i) const { return (&x)[i]; }

	void Set(float nx, float ny) { x = nx; y = ny; }

	[[nodiscard]] float GetLength() const;
	[[nodiscard]] float GetLengthSquared() const;
	[[nodiscard]] Vector2 Normalize() const;
	[[nodiscard]] float Distance(const Vector2& v) const;
	[[nodiscard]] float DotProduct(const Vector2& v) const;

	float x = 0.0f;
	float y = 0.0f;
};

inline float Vector2::GetLength() const
{
	return sqrt(x * x + y * y);
}

inline float Vector2::GetLengthSquared() const
{
	return x * x + y * y;
}

inline Vector2 Vector2::Normalize() const
{
	const float invLen = 1.0f / sqrt(x * x + y * y);
	return { x * invLen, y * invLen };
}

inline float Vector2::Distance(const Vector2& v) const
{
	return (*this - v).GetLength();
}

inline float Vector2::DotProduct(const Vector2& v) const
{
	return x * v.x + y * v.y;
}

inline Vector2 operator*(float lhs, const Vector2& rhs) { return rhs * lhs; }
inline Vector2 operator+(float lhs, const Vector2& rhs) { return rhs + lhs; }

//=============================================================================
// Vector3
//=============================================================================

struct Vector3
{
public:
	constexpr Vector3() = default;
	constexpr Vector3(Vector3&&) = default;
	constexpr Vector3(const Vector3&) = default;
	constexpr Vector3(float n) : x(n), y(n), z(n) {}
	constexpr Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

	constexpr Vector3& operator=(Vector3&&) = default;
	constexpr Vector3& operator=(const Vector3&) = default;

	//bool operator==(const Vector3& v);
	//bool operator!=(const Vector3& v);

	Vector3 operator+(float f) const { return { x + f, y + f, z + f }; }
	Vector3 operator+(const Vector3& v) const { return { x + v.x, y + v.y, z + v.z }; }
	Vector3 operator-() const { return { -x, -y, -z }; }
	Vector3 operator-(float f) const { return { x - f, y - f, z - f }; }
	Vector3 operator-(const Vector3& v) const { return { x - v.x, y - v.y, z - v.z }; }
	Vector3 operator*(float f) const { return { x * f, y * f, z * f }; }
	Vector3 operator*(const Vector3& v) const { return { x * v.x, y * v.y, z * v.z }; }
	Vector3 operator/(float f) const { return { x / f, y / f, z / f }; }
	Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return(*this); }
	Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return(*this); }
	Vector3& operator*=(float n) { x *= n; y *= n; z *= n; return(*this); }
	Vector3& operator/=(float n) { x /= n; y /= n; z /= n; return(*this); }

	float* operator&() { return (float*)this; }
	operator float*() { return &x; }
	operator const float*() const { return &x; }

	float operator[](size_t i) { return (&x)[i]; }
	const float operator[](size_t i) const { return (&x)[i]; }

	void Set(float nx, float ny, float nz) { x = nx; y = ny; z = nz; }

	[[nodiscard]] float GetLength() const;
	[[nodiscard]] float GetLengthSquared() const;
	[[nodiscard]] float Distance(const Vector3& v) const;
	[[nodiscard]] Vector3 Normalize() const;
	[[nodiscard]] float DotProduct(const Vector3& v) const;
	[[nodiscard]] Vector3 CrossProduct(const Vector3& vector) const;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

inline float Vector3::GetLength() const
{
	return sqrt(x * x + y * y + z * z);
}

inline float Vector3::GetLengthSquared() const
{
	return x * x + y * y + z * z;
}

inline float Vector3::Distance(const Vector3& v) const
{
	return (*this - v).GetLength();
}

inline Vector3 Vector3::Normalize() const
{
	const float invLen = 1.0f / sqrt(x * x + y * y + z * z);
	return { x * invLen, y * invLen, z * invLen };
}

inline float Vector3::DotProduct(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

inline Vector3 Vector3::CrossProduct(const Vector3& v) const
{
	return { 
		y * v.z - z * v.y, 
		z * v.x - x * v.z, 
		x * v.y - y * v.x };
}

inline Vector3 operator+(float lhs, const Vector3& rhs) { return rhs + lhs; }
inline Vector3 operator*(float lhs, const Vector3& rhs) { return rhs * lhs; }

//=============================================================================
// Vector4
//=============================================================================

struct Vector4
{
public:
	constexpr Vector4() = default;
	constexpr Vector4(Vector4&&) = default;
	constexpr Vector4(const Vector4&) = default;
	constexpr Vector4(float n) : x(n), y(n), z(n), w(n) {}
	constexpr Vector4(float nx, float ny, float nz, float nw) : x(nx), y(ny), z(nz), w(nw) {}

	constexpr Vector4& operator=(Vector4&&) = default;
	constexpr Vector4& operator=(const Vector4&) = default;

	Vector4 operator+(const Vector4& a) const { return { x + a.x, y + a.y, z + a.z, w + a.w }; }
	Vector4 operator-(const Vector4& a) const { return { x - a.x, y - a.y, z - a.z, w - a.w }; }
	Vector4 operator-() const { return { -x, -y, -z, -w }; }
	Vector4 operator*(const Vector4& a) const { return { x * a.x, y * a.y, z * a.z , w * a.w}; }
	Vector4 operator*(float n) const { return { x * n, y * n, z * n, w * n }; }
	Vector4 operator/(float n) const { return { x / n, y / n, z / n, w / n }; }
	Vector4& operator+=(const Vector4& a) { x += a.x; y += a.y; z += a.z; w += a.w; return(*this); }
	Vector4& operator-=(const Vector4& a) { x -= a.x; y -= a.y; z -= a.z; w -= a.w;  return(*this); }
	Vector4& operator*=(float n) { x *= n; y *= n; z *= n; w *= n; return(*this); }
	Vector4& operator/=(float n) { x /= n; y /= n; z /= n; w /= n; return(*this); }

	//bool operator==(const Vector4& a);
	//bool operator!=(const Vector4& a);

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
};

//=============================================================================
// Matrix4
//=============================================================================

struct Matrix4
{
public:
	constexpr Matrix4() = default;
	constexpr Matrix4(Matrix4&&) = default;
	constexpr Matrix4(const Matrix4&) = default;
	constexpr Matrix4(const glm::mat4& m);

	float& operator[](int Index) { return m[Index]; }
	const float& operator[](int Index) const { return m[Index]; }

	void Translate(const Vector3& pos);
	void Rotate(float angle, const Vector3& rot);

	float m[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f };
};

inline constexpr Matrix4::Matrix4(const glm::mat4& mat)
{
	m[0] = mat[0][0];
	m[1] = mat[0][1];
	m[2] = mat[0][2];
	m[3] = mat[0][3];
	m[4] = mat[1][0];
	m[5] = mat[1][1];
	m[6] = mat[1][2];
	m[7] = mat[1][3];
	m[8] = mat[2][0];
	m[9] = mat[2][1];
	m[10] = mat[2][2];
	m[11] = mat[2][3];
	m[12] = mat[3][0];
	m[13] = mat[3][1];
	m[14] = mat[3][2];
	m[15] = mat[3][3];
}

inline void Matrix4::Translate(const Vector3& pos)
{
	m[12] = pos.x;
	m[13] = pos.y;
	m[14] = pos.z;
}

inline void Matrix4::Rotate(float angle, const Vector3& v)
{
	const float a = angle;
	const float c = cos(a);
	const float s = sin(a);

	Vector3 axis = v.Normalize();
	Vector3 temp(axis * (1.0f - c));

	Matrix4 rotate;
	rotate.m[0] = c + temp.x * axis.x;
	rotate.m[1] = temp.x * axis.y + s * axis.z;
	rotate.m[2] = temp.x * axis.z - s * axis.y;

	rotate.m[4] = temp.y * axis.x - s * axis.z;
	rotate.m[5] = c + temp.y * axis.y;
	rotate.m[6] = temp.y * axis.z + s * axis.x;

	rotate.m[8] = temp.z * axis.x + s * axis.y;
	rotate.m[9] = temp.z * axis.y - s * axis.x;
	rotate.m[10] = c + temp.z * axis.z;

	Vector4 v1 = Vector4{ m[0], m[1], m[2], m[3] } * rotate.m[0] 
		+ Vector4{ m[4], m[5], m[6], m[7] } * rotate.m[1] 
		+ Vector4{ m[8], m[9], m[10], m[11] } * rotate.m[2];
	Vector4 v2 = Vector4{ m[0], m[1], m[2], m[3] } *rotate.m[3]
		+ Vector4{ m[4], m[5], m[6], m[7] } *rotate.m[4]
		+ Vector4{ m[8], m[9], m[10], m[11] } *rotate.m[5];
	Vector4 v3 = Vector4{ m[0], m[1], m[2], m[3] } *rotate.m[6]
		+ Vector4{ m[4], m[5], m[6], m[7] } *rotate.m[7]
		+ Vector4{ m[8], m[9], m[10], m[11] } *rotate.m[8];


	/*mat<4, 4, T, Q> Result;
	Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
	Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
	Result[3] = m[3];
	return Result;*/
}


//template<typename T, qualifier Q>
//GLM_FUNC_QUALIFIER mat<4, 4, T, Q> rotate(mat<4, 4, T, Q> const& m, T angle, vec<3, T, Q> const& v)
//{
//	vec<3, T, Q> axis(normalize(v));
//	vec<3, T, Q> temp((T(1) - c) * axis);
//
//	mat<4, 4, T, Q> Rotate;
//	Rotate[0][0] = c + temp[0] * axis[0];
//	Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
//	Rotate[0][2] = temp[0] * axis[2] - s * axis[1];
//
//	Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
//	Rotate[1][1] = c + temp[1] * axis[1];
//	Rotate[1][2] = temp[1] * axis[2] + s * axis[0];
//
//	Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
//	Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
//	Rotate[2][2] = c + temp[2] * axis[2];
//
//	mat<4, 4, T, Q> Result;
//	Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
//	Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
//	Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
//	Result[3] = m[3];
//	return Result;
//}