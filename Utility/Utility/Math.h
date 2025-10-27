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

	Float3 operator-(const Float3& rhs)
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
}

struct __declspec(align(16)) Float4x4
{
	Float4 r[4];
};

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
UTILITY_API void VectorToEulerDeg(Float4& out, const Float3& lhs);
//UTILITY_API void ForwardToEulerDeg(Float3& outEulerDeg, const Float3& forward);

UTILITY_API void QuaternionToEulerDeg(Float4& out, const Float4& Q);
UTILITY_API void QuaternionToEulerRad(Float4& out, const Float4& Q);
//UTILITY_API void RotationToQuaternion(Float4& outQ, const Float4& rot);
//UTILITY_API void QuaternionRotaionRollPitchYaw(Float4& outQ, const Float4& angle);


UTILITY_API void MatrixIdentity(Float4x4& out);
UTILITY_API void MatrixTranspose(Float4x4& out, const Float4x4& src);
UTILITY_API void MatrixInverse(Float4x4& out, const Float4x4& src);
UTILITY_API void MatrixCompose(Float4x4& out, const Float4& scale, const Float4& rotAngle, const Float4& pos);
UTILITY_API void MatrixDecompose(const Float4x4& matrx, Float4& scale, Float4& rotQ, Float4& pos);
//UTILITY_API void MatrixDecomposeFromRotQ(const Float4x4& matrx, Float4& rotQ);
UTILITY_API void MatrixLookAtLH(Float4x4& out, const Float4& eyePos, const Float4& focusPos, const Float4& eyeUp);
UTILITY_API void MatrixLookToLH(Float4x4& out, const Float4& eyePos, const Float4& eyeDir, const Float4& eyeUp);
UTILITY_API void MatrixPerspectiveFovLH(Float4x4& out, float fovDegAngle, float aspectRatio, float nearZ, float farZ);

UTILITY_API float ConvertDegToRad(float deg);
UTILITY_API float ConvertRadToDeg(float rad);