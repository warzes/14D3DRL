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

inline float Vector2::GetLength() const
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
	constexpr Vector3() = default;
	constexpr Vector3(Vector3&&) = default;
	constexpr Vector3(const Vector3&) = default;
	constexpr Vector3(float n) : x(n), y(n), z(n) {}
	constexpr Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

	constexpr Vector3& operator=(Vector3&&) = default;
	constexpr Vector3& operator=(const Vector3&) = default;

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

inline float Vector3::GetLength() const
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
	Vector4& operator/=(float n) { x /= n; y /= n; z /= n; w / n; return(*this); }

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