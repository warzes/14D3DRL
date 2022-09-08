#pragma once

//=============================================================================
// Math core
//=============================================================================
namespace math
{
	constexpr double RoundingError64 = 0.00000001;   // 1.0e-8 (alt. DBL_EPSILON)
	constexpr float RoundingError    = 0.000001f;    // 1.0e-6 (alt. FLT_EPSILON)

	inline bool Equal(float A, float B)
	{
		return fabs(A - B) < RoundingError;
	}

} // namespace math

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

	// TODO: проверить эти операции и отрефакторить
	[[nodiscard]] bool operator==(const Vector2& v) const { return math::Equal(x, v.x) && math::Equal(y, v.y); }
	[[nodiscard]] bool operator!=(const Vector2& v) const { return !math::Equal(x, v.x) || !math::Equal(y, v.y); }
	[[nodiscard]] bool operator>(const Vector2& v) const { return x > v.x && y > v.y; }
	[[nodiscard]] bool operator<(const Vector2& v) const { return x < v.x && y < v.y; }
	[[nodiscard]] bool operator>=(const Vector2& v) const { return *this > v || *this == v; }
	[[nodiscard]] bool operator<=(const Vector2 & v) const { return *this < v || *this == v; }
	
	Vector2 operator+(float f) const { return { x + f, y + f }; }
	Vector2 operator+(const Vector2& v) const { return { x + v.x, y + v.y }; }	
	Vector2 operator-() const { return { -x, -y }; }
	Vector2 operator-(float f) const { return { x - f, y - f }; }
	Vector2 operator-(const Vector2& v) const { return { x - v.x, y - v.y }; }	
	Vector2 operator*(float f) const { return { x * f, y * f }; }
	Vector2 operator*(const Vector2& v) const { return { x * v.x, y * v.y }; }	
	Vector2 operator/(float f) const { return { x / f, y / f }; }
	Vector2 operator/(const Vector2& v) const { return { x / v.x, y / v.y }; }

	Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return(*this); }
	Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return(*this); }
	Vector2& operator*=(float f) { x *= f; y *= f; return(*this); }
	Vector2& operator*=(const Vector2& v) { x *= v.x; y *= v.y; return(*this); }
	Vector2& operator/=(float f) { x /= f; y /= f; return(*this); }
	Vector2& operator/=(const Vector2& v) { x /= v.x; y /= v.y; return(*this); }

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

	// TODO: проверить эти операции и отрефакторить
	[[nodiscard]] bool operator==(const Vector3& v) const { return math::Equal(x, v.x) && math::Equal(y, v.y) && math::Equal(z, v.z); }
	[[nodiscard]] bool operator!=(const Vector3& v) const { return !math::Equal(x, v.x) || !math::Equal(y, v.y) || !math::Equal(z, v.z); }
	[[nodiscard]] bool operator>(const Vector3& v) const { return x > v.x && y > v.y && z > v.z; }
	[[nodiscard]] bool operator<(const Vector3& v) const { return x < v.x && y < v.y && z < v.z; }
	[[nodiscard]] bool operator>=(const Vector3& v) const { return *this > v || *this == v; }
	[[nodiscard]] bool operator<=(const Vector3& v) const { return *this < v || *this == v; }

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
	Matrix4(
		float  m0, float  m1, float  m2, float  m3,
		float  m4, float  m5, float  m6, float  m7,
		float  m8, float  m9, float m10, float m11,
		float m12, float m13, float m14, float m15)
	{
		m[0] = m0;  m[1] = m1;  m[2] = m2;  m[3] = m3;
		m[4] = m4;  m[5] = m5;  m[6] = m6;  m[7] = m7;
		m[8] = m8;  m[9] = m9;  m[10] = m10; m[11] = m11;
		m[12] = m12; m[13] = m13; m[14] = m14; m[15] = m15;
	}

	const Matrix4 operator+(const Matrix4& M) const
	{
		return
		{
			m[0] + M[0], m[1] + M[1], m[2] + M[2], m[3] + M[3],
			m[4] + M[4], m[5] + M[5], m[6] + M[6], m[7] + M[7],
			m[8] + M[8], m[9] + M[9], m[10] + M[10], m[11] + M[11],
			m[12] + M[12], m[13] + M[13], m[14] + M[14], m[15] + M[15]
		};
	}
	const Matrix4 operator-(const Matrix4& M) const
	{
		return
		{
			m[0] - M[0], m[1] - M[1], m[2] - M[2], m[3] - M[3],
			m[4] - M[4], m[5] - M[5], m[6] - M[6], m[7] - M[7],
			m[8] - M[8], m[9] - M[9], m[10] - M[10], m[11] - M[11],
			m[12] - M[12], m[13] - M[13], m[14] - M[14], m[15] - M[15]
		};
	}
	const Matrix4 operator*(const Matrix4& M) const
	{
		return
		{
			m[0] * M[0] + m[1] * M[4] + m[2] * M[8] + m[3] * M[12],
			m[0] * M[1] + m[1] * M[5] + m[2] * M[9] + m[3] * M[13],
			m[0] * M[2] + m[1] * M[6] + m[2] * M[10] + m[3] * M[14],
			m[0] * M[3] + m[1] * M[7] + m[2] * M[11] + m[3] * M[15],
			m[4] * M[0] + m[5] * M[4] + m[6] * M[8] + m[7] * M[12],
			m[4] * M[1] + m[5] * M[5] + m[6] * M[9] + m[7] * M[13],
			m[4] * M[2] + m[5] * M[6] + m[6] * M[10] + m[7] * M[14],
			m[4] * M[3] + m[5] * M[7] + m[6] * M[11] + m[7] * M[15],
			m[8] * M[0] + m[9] * M[4] + m[10] * M[8] + m[11] * M[12],
			m[8] * M[1] + m[9] * M[5] + m[10] * M[9] + m[11] * M[13],
			m[8] * M[2] + m[9] * M[6] + m[10] * M[10] + m[11] * M[14],
			m[8] * M[3] + m[9] * M[7] + m[10] * M[11] + m[11] * M[15],
			m[12] * M[0] + m[13] * M[4] + m[14] * M[8] + m[15] * M[12],
			m[12] * M[1] + m[13] * M[5] + m[14] * M[9] + m[15] * M[13],
			m[12] * M[2] + m[13] * M[6] + m[14] * M[10] + m[15] * M[14],
			m[12] * M[3] + m[13] * M[7] + m[14] * M[11] + m[15] * M[15]
		};
	}
	const Matrix4 operator*(float f) const
	{
		return
		{
			m[0] * f, m[1] * f, m[2] * f, m[3] * f,
			m[4] * f, m[5] * f, m[6] * f, m[7] * f,
			m[8] * f, m[9] * f, m[10] * f, m[11] * f,
			m[12] * f, m[13] * f, m[14] * f, m[15] * f
		};
	}
	const Matrix4 operator/(float f) const
	{
		return
		{
			m[0] / f, m[1] / f, m[2] / f, m[3] / f,
			m[4] / f, m[5] / f, m[6] / f, m[7] / f,
			m[8] / f, m[9] / f, m[10] / f, m[11] / f,
			m[12] / f, m[13] / f, m[14] / f, m[15] / f
		};
	}

	Matrix4& operator+=(const Matrix4& M)
	{
		m[0] += M[0]; m[1] += M[1]; m[2] += M[2]; m[3] += M[3];
		m[4] += M[4]; m[5] += M[5]; m[6] += M[6]; m[7] += M[7];
		m[8] += M[8]; m[9] += M[9]; m[10] += M[10]; m[11] += M[11];
		m[12] += M[12]; m[13] += M[13]; m[14] += M[14]; m[15] += M[15];
		return *this;
	}
	Matrix4& operator-=(const Matrix4& M)
	{
		m[0] -= M[0]; m[1] -= M[1]; m[2] -= M[2]; m[3] -= M[3];
		m[4] -= M[4]; m[5] -= M[5]; m[6] -= M[6]; m[7] -= M[7];
		m[8] -= M[8]; m[9] -= M[9]; m[10] -= M[10]; m[11] -= M[11];
		m[12] -= M[12]; m[13] -= M[13]; m[14] -= M[14]; m[15] -= M[15];
		return *this;
	}
	Matrix4& operator*=(const Matrix4& M)
	{
		Set(m[0] * M[0] + m[1] * M[4] + m[2] * M[8] + m[3] * M[12],
			m[0] * M[1] + m[1] * M[5] + m[2] * M[9] + m[3] * M[13],
			m[0] * M[2] + m[1] * M[6] + m[2] * M[10] + m[3] * M[14],
			m[0] * M[3] + m[1] * M[7] + m[2] * M[11] + m[3] * M[15],
			m[4] * M[0] + m[5] * M[4] + m[6] * M[8] + m[7] * M[12],
			m[4] * M[1] + m[5] * M[5] + m[6] * M[9] + m[7] * M[13],
			m[4] * M[2] + m[5] * M[6] + m[6] * M[10] + m[7] * M[14],
			m[4] * M[3] + m[5] * M[7] + m[6] * M[11] + m[7] * M[15],
			m[8] * M[0] + m[9] * M[4] + m[10] * M[8] + m[11] * M[12],
			m[8] * M[1] + m[9] * M[5] + m[10] * M[9] + m[11] * M[13],
			m[8] * M[2] + m[9] * M[6] + m[10] * M[10] + m[11] * M[14],
			m[8] * M[3] + m[9] * M[7] + m[10] * M[11] + m[11] * M[15],
			m[12] * M[0] + m[13] * M[4] + m[14] * M[8] + m[15] * M[12],
			m[12] * M[1] + m[13] * M[5] + m[14] * M[9] + m[15] * M[13],
			m[12] * M[2] + m[13] * M[6] + m[14] * M[10] + m[15] * M[14],
			m[12] * M[3] + m[13] * M[7] + m[14] * M[11] + m[15] * M[15]);
		return *this;
	}
	Matrix4& operator*=(float f)
	{
		m[0] *= f; m[1] *= f; m[2] *= f; m[3] *= f;
		m[4] *= f; m[5] *= f; m[6] *= f; m[7] *= f;
		m[8] *= f; m[9] *= f; m[10] *= f; m[11] *= f;
		m[12] *= f; m[13] *= f; m[14] *= f; m[15] *= f;
		return *this;
	}
	Matrix4& operator/=(float f)
	{
		m[0] /= f; m[1] /= f; m[2] /= f; m[3] /= f;
		m[4] /= f; m[5] /= f; m[6] /= f; m[7] /= f;
		m[8] /= f; m[9] /= f; m[10] /= f; m[11] /= f;
		m[12] /= f; m[13] /= f; m[14] /= f; m[15] /= f;
		return *this;
	}

	operator float* () { return &m[0]; }
	operator const float* () const { return &m[0]; }

	float& operator[](int Index) { return m[Index]; }
	const float& operator[](int Index) const { return m[Index]; }

	void Set(const float* f)
	{
		m[0] = f[0];  m[1] = f[1];  m[2] = f[2];  m[3] = f[3];
		m[4] = f[4];  m[5] = f[5];  m[6] = f[6];  m[7] = f[7];
		m[8] = f[8];  m[9] = f[9];  m[10] = f[10]; m[11] = f[11];
		m[12] = f[12]; m[13] = f[13]; m[14] = f[14]; m[15] = f[15];
	}
	void Set(
		float  m0, float  m1, float  m2, float  m3,
		float  m4, float  m5, float  m6, float  m7,
		float  m8, float  m9, float m10, float m11,
		float m12, float m13, float m14, float m15)
	{
		m[0] = m0;  m[1] = m1;  m[2] = m2;  m[3] = m3;
		m[4] = m4;  m[5] = m5;  m[6] = m6;  m[7] = m7;
		m[8] = m8;  m[9] = m9;  m[10] = m10; m[11] = m11;
		m[12] = m12; m[13] = m13; m[14] = m14; m[15] = m15;
	}
	void Set(const Matrix4& M)
	{
		m[0] = M[0];  m[1] = M[1];   m[2] = M[2];  m[3] = M[3];
		m[4] = M[4];  m[5] = M[5];   m[6] = M[6];  m[7] = M[7];
		m[8] = M[8];  m[9] = M[9];   m[10] = M[10]; m[11] = M[11];
		m[12] = M[12]; m[13] = M[13]; m[14] = M[14]; m[15] = M[15];
	}

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

inline Matrix4 Orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	const float tx = -(right + left) / (right - left),
		ty = -(top + bottom) / (top - bottom),
		tz = -(zFar + zNear) / (zFar - zNear);

	return {
		2 / (right - left), 0,                  0,                  tx,
		0,                  2 / (top - bottom), 0,                  ty,
		0,                  0,                 -2 / (zFar - zNear), tz,
		0,                  0,                  0,                  1 };
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