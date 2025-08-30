#include "stdafx.h"
#include "Float4.h"

const Float4 Float4::Front = { 1.0f,  0.0f,  0.0f };
const Float4 Float4::Back = { -1.0f,  0.0f,  0.0f };
const Float4 Float4::Left = { 0.0f, -1.0f,  0.0f };
const Float4 Float4::Right = { 0.0f,  1.0f,  0.0f };
const Float4 Float4::Up = { 0.0f,  0.0f,  1.0f };
const Float4 Float4::Down = { 0.0f,  0.0f, -1.0f };

Float4::Float4()
	: X(0.0f),
	Y(0.0f),
	Z(0.0f),
	W(1.0f)
{
}

Float4::Float4(float x, float y)
	: X(x),
	Y(y),
	Z(0.0f),
	W(1.0f)
{
}

Float4::Float4(float x, float y, float z)
	: X(x),
	Y(y),
	Z(z),
	W(1.0f)
{
}

Float4::Float4(float x, float y, float z, float w)
	:X(X),
	Y(y),
	Z(z),
	W(w)
{
}

Float4::Float4(const DirectX::XMFLOAT2& other)
	: X(other.x), Y(other.y), Z(0.0f), W(0.0f)
{
}

Float4::Float4(const DirectX::XMFLOAT3& other)
	: X(other.x), Y(other.y), Z(other.z), W(1.0f)
{
}

Float4::Float4(const DirectX::XMFLOAT4& other)
	: X(other.x), Y(other.y), Z(other.z), W(other.w)
{
}

Float4& Float4::operator+=(const Float4& value)
{
	X += value.X;
	Y += value.Y;
	Z += value.Z;
	W = 1.0f;
	/*W += value.W;
	W = max(0.0f, min(1.0f, W));*/
	return *this;
}

Float4 Float4::operator+(const Float4& value) const
{
	Float4 Result = *this;
	Result.X += value.X;
	Result.Y += value.Y;
	Result.Z += value.Z;
	Result.W = 1.0f;
	/*Result.W += value.W;
	Result.W = max(0.0f, min(1.0f, Result.W));*/
	return Result;
}

Float4 Float4::operator+(float value) const
{
	Float4 Result = *this;
	Result.X += value;
	Result.Y += value;
	Result.Z += value;
	Result.W = 1.0f;
	/*Result.W += value;
	Result.W = max(0.0f, min(1.0f, Result.W));*/
	return Result;
}

Float4& Float4::operator-=(const Float4& value)
{
	X -= value.X;
	Y -= value.Y;
	Z -= value.Z;
	W = 1.0f;
	/*W -= value.W;
	W = max(0.0f, min(1.0f, W));*/
	return *this;
}

Float4 Float4::operator-(const Float4& value) const
{
	Float4 Result = *this;
	Result.X -= value.X;
	Result.Y -= value.Y;
	Result.Z -= value.Z;
	Result.W = 1.0f;
	/*Result.W -= value.W;
	Result.W = max(0.0f, min(1.0f, Result.W));*/
	return Result;
}

Float4 Float4::operator-(float value) const
{
	Float4 Result = *this;
	Result.X -= value;
	Result.Y -= value;
	Result.Z -= value;
	Result.W = 1.0f;
	/*Result.W -= value;
	Result.W = max(0.0f, min(1.0f, Result.W));*/
	return Result;
}

Float4 Float4::operator-() const
{
	Float4 Result = *this;
	Result.X = -Result.X;
	Result.Y = -Result.Y;
	Result.Z = -Result.Z;
	//Result.W = Result.W;
	return Result;
}

Float4 Float4::operator*(float value) const
{
	Float4 Result = *this;
	Result.X *= value;
	Result.Y *= value;
	Result.Z *= value;
	Result.W = 1.0f;
	/*Result.W *= value;
	Result.W = max(0.0f, min(1.0f, Result.W));*/
	return Result;
}

Float4 Float4::operator*(const Float4& value) const
{
	Float4 Result = *this;
	Result.X *= value.X;
	Result.Y *= value.Y;
	Result.Z *= value.Z;
	Result.W = 1.0f;
	/*Result.W *= value.W;
	Result.W = max(0.0f, min(1.0f, Result.W));*/
	return Result;
}

bool Float4::operator==(const Float4& value) const
{
	return X == value.X && Y == value.Y && Z == value.Z;
}

Float4 Float4::Normalize() const
{
	Float4 normalFloat;
	normalFloat.dxVector = DirectX::XMVector3Normalize(dxVector);
	return normalFloat;
}

float Float4::HalfX() const
{
	return X * 0.5f;
}

float Float4::HalfY() const
{
	return Y * 0.5f;
}

float Float4::Size() const
{
	return std::sqrtf((X * X) + (Y * Y) + (Z * Z));
}
