#pragma once

struct Float4 {
	union {
		struct {
			float X;
			float Y;
			float Z;
			float W;
		};
		struct {
			float R;
			float G;
			float B;
			float A;
		};
		float Arr1D[4];
	};

	Float4()
		: X(0.0f), Y(0.0f), Z(0.0f), W(1.0f) {
	}

	Float4(float x, float y, float z)
		: X(x), Y(y), Z(z), W(1.0f) {
	}

	Float4(float x, float y, float z, float w)
		: X(x), Y(y), Z(z), W(w) {
	}

	Float4 operator+(const Float4& rhs)
	{
		return { this->X + rhs.X, this->Y + rhs.Y, this->Z + rhs.Z, this->W + rhs.W };
	}
	Float4& operator+=(const Float4& rhs)
	{
		this->X += rhs.X;
		this->Y += rhs.Y;
		this->Z += rhs.Z;
		this->W += rhs.W;

		return *this;
	}

	Float4 operator-(const Float4& rhs)
	{
		return { this->X - rhs.X, this->Y - rhs.Y, this->Z - rhs.Z,  this->W - rhs.W };
	}

	Float4& operator-=(const Float4& rhs)
	{
		this->X -= rhs.X;
		this->Y -= rhs.Y;
		this->Z -= rhs.Z;
		this->W -= rhs.W;
		return *this;
	}

	Float4 operator*(const Float4& rhs)
	{
		return { this->X * rhs.X, this->Y * rhs.Y, this->Z * rhs.Z,this->W * rhs.W };
	}

	Float4& operator*=(const Float4& rhs)
	{
		this->X *= rhs.X;
		this->Y *= rhs.Y;
		this->Z *= rhs.Z;
		this->W *= rhs.W;

		return *this;
	}
};

struct Float3 {
	union {
		struct {
			float X;
			float Y;
			float Z;
		};
		float Arr1D[3];
	};

	Float3()
		: X(0.0f), Y(0.0f), Z(0.0f) {
	}

	Float3(float x, float y, float z)
		: X(x), Y(y), Z(z) {
	}

	Float3 operator+(const Float3& rhs)
	{
		return { this->X + rhs.X, this->Y + rhs.Y, this->Z + rhs.Z };
	}

	Float3& operator+=(const Float3& rhs)
	{
		this->X += rhs.X;
		this->Y += rhs.Y;
		this->Z += rhs.Z;

		return *this;
	}

	Float3 operator-(const Float3& rhs) const
	{
		return { this->X - rhs.X, this->Y - rhs.Y, this->Z - rhs.Z };
	}

	Float3& operator-=(const Float3& rhs)
	{
		this->X -= rhs.X;
		this->Y -= rhs.Y;
		this->Z -= rhs.Z;
		return *this;
	}

	Float3 operator*(const Float3& rhs)
	{
		return { this->X * rhs.X, this->Y * rhs.Y, this->Z * rhs.Z };
	}

	Float3& operator*=(const Float3& rhs)
	{
		this->X *= rhs.X;
		this->Y *= rhs.Y;
		this->Z *= rhs.Z;

		return *this;
	}

	Float3 operator/(const Float3& rhs)
	{
		return { this->X / rhs.X, this->Y / rhs.Y, this->Z / rhs.Z };
	}

	Float3& operator/=(const Float3& rhs)
	{
		this->X /= rhs.X;
		this->Y /= rhs.Y;
		this->Z /= rhs.Z;

		return *this;
	}

};

struct Float2 {
	union {
		struct {
			float X;
			float Y;
		};
		float Arr1D[2];
	};

	Float2()
		: X(0.0f), Y(0.0f) {
	}

	Float2(float x, float y)
		: X(x), Y(y) {
	}

	Float2 operator+(const Float2& rhs)
	{
		return { this->X + rhs.X, this->Y + rhs.Y };
	}
	Float2& operator+=(const Float2& rhs)
	{
		this->X += rhs.X;
		this->Y += rhs.Y;

		return *this;
	}

	Float2 operator-(const Float2& rhs)
	{
		return { this->X - rhs.X, this->Y - rhs.Y };
	}

	Float2& operator-=(const Float2& rhs)
	{
		this->X -= rhs.X;
		this->Y -= rhs.Y;

		return *this;
	}

	Float2 operator*(const Float2& rhs)
	{
		return { this->X * rhs.X, this->Y * rhs.Y };
	}

	Float2& operator*=(const Float2& rhs)
	{
		this->X *= rhs.X;
		this->Y *= rhs.Y;

		return *this;
	}
};

typedef Float4 Vector;
typedef Float4 Color;

struct __declspec(align(16)) Float4x4
{
	Float4 r[4];
};

struct __declspec(align(16)) Float3x3
{
	Float4 r[3]; // W는 패딩 (쓰지 않음)
};


namespace MATH
{
	const static float PI = 3.141592654f;
	const static float DegToRadFloat = PI / 180.0f;
	const static float RadToDegFloat = 180.0f / PI;

	static const Float4 Left = { 0.0f, -1.0f,  0.0f, 0.0f };
	static const Float4 Right = { 0.0f,  1.0f,  0.0f, 0.0f };
	static const Float4 Up = { 0.0f,  0.0f,  1.0f, 0.0f };
	static const Float4 Down = { 0.0f,  0.0f, -1.0f, 0.0f };
	static const Float4 Front = { 1.0f,  0.0f,  0.0f, 0.0f };
	static const Float4 Back = { -1.0f,  0.0f,  0.0f, 0.0f };

	static const Float4 DegToRad(DegToRadFloat, DegToRadFloat, DegToRadFloat, 1.0f);

	UTILITY_API void VectorDot(float& out, const Float4& lhs, const Float4& rhs);
	UTILITY_API void VectorCross(Float4& out, const Float4& lhs, const Float4& rhs);
	UTILITY_API void VectorAdd(Float4& out, const Float4& lhs, const Float4& rhs);
	UTILITY_API void VectorSub(Float4& out, const Float4& lhs, const Float4& rhs);
	UTILITY_API void VectorMultiply(Float4& out, const Float4& lhs, const Float4& rhs);
	UTILITY_API void VectorScale(Float4& out, const Float4& lhs, float scale);
	UTILITY_API void VectorNormalize(Float4& out, const Float4& lhs);
	UTILITY_API float VectorLength(const Float4& lhs);

	UTILITY_API void VectorDot(float& out, const Float3& lhs, const Float3& rhs);
	UTILITY_API void VectorCross(Float3& out, const Float3& lhs, const Float3& rhs);
	UTILITY_API void VectorAdd(Float3& out, const Float3& lhs, const Float3& rhs);
	UTILITY_API void VectorSub(Float3& out, const Float3& lhs, const Float3& rhs);
	UTILITY_API void VectorMultiply(Float3& out, const Float3& lhs, const Float3& rhs);
	UTILITY_API void VectorScale(Float3& out, const Float3& lhs, float scale);
	UTILITY_API void VectorNormalize(Float3& out, const Float3& lhs);
	UTILITY_API float VectorLength(const Float3& lhs);
	UTILITY_API void VectorToEulerDeg(Float4& out, const Float3& vector);



	UTILITY_API void QuaternionToEulerDeg(Float4& out, const Float4& quat);
	UTILITY_API void QuaternionToEulerRad(Float4& out, const Float4& quat);


	UTILITY_API void CreateMatrixFromRows(Float3x3& out, const Float3& row0, const Float3& row1, const Float3& row2);
	UTILITY_API void CreateMatrixFromCols(Float3x3& out, const Float3& col0, const Float3& col1, const Float3& col2);
	UTILITY_API void CreateMatrixFromRows(Float4x4& out, const Float4& row0, const Float4& row1, const Float4& row2, const Float4& row3);
	UTILITY_API void CreateMatrixFromCols(Float4x4& out, const Float4& col0, const Float4& col1, const Float4& col2, const Float4& col3);

	UTILITY_API void MatrixIdentity(Float4x4& out);
	UTILITY_API void MatrixMultiply(Float4& out, const Float4x4& lhs, const Float4& rhs);
	UTILITY_API void MatrixMultiply(Float4& out, const Float4& lhs, const Float4x4& rhs);
	UTILITY_API void MatrixMultiply(Float4x4& out, const Float4x4& lhs, const Float4x4& rhs);
	UTILITY_API void MatrixTranspose(Float4x4& out, const Float4x4& src);
	UTILITY_API void MatrixInverse(Float4x4& out, const Float4x4& src);
	UTILITY_API void MatrixDeterminant(float& out, const Float4x4& src);
	UTILITY_API void MatrixDeterminant(float& out, const Float3x3& src);
	UTILITY_API void MatrixCompose(Float4x4& out, const Float4& scale, const Float4& rotDeg, const Float4& pos);
	UTILITY_API void MatrixDecompose(Float4& outScale, Float4& outQuat, Float4& outPos, const Float4x4& src);
	UTILITY_API void MatrixLookAtLH(Float4x4& out, const Float4& eyePos, const Float4& focusPos, const Float4& eyeUp);
	UTILITY_API void MatrixLookToLH(Float4x4& out, const Float4& eyePos, const Float4& eyeForward, const Float4& eyeUp);
	UTILITY_API void MatrixPerspectiveFovLH(Float4x4& out, float fovDegAngle, float aspectRatio, float nearZ, float farZ);


	UTILITY_API float ConvertDegToRad(float deg);
	UTILITY_API float ConvertRadToDeg(float rad);


	UTILITY_API bool Intersection3D_Ray_Triangle
	(
		float* pOutT,
		const Float3& rayPos, const Float3& rayDir,
		const Float3& triA, const Float3& triB, const Float3& triC
	);
}