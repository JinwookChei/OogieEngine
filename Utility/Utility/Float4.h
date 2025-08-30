#pragma once

class UTILITY_API Float4
{
public:
	static const Float4 Front;
	static const Float4 Back;
	static const Float4 Left;
	static const Float4 Right;
	static const Float4 Up;
	static const Float4 Down;

	union 
	{
		struct 
		{
			float X;
			float Y;
			float Z;
			float W;
		};
		struct 
		{
			float R;
			float G;
			float B;
			float A;
		};
		float Array1D[4];

		DirectX::XMVECTOR dxVector;
	};

	Float4();

	Float4(float x, float y);

	Float4(float x, float y, float z);

	Float4(float x, float y, float z, float w);

	Float4(const DirectX::XMFLOAT2& other);

	Float4(const DirectX::XMFLOAT3& other);

	Float4(const DirectX::XMFLOAT4& other);

	Float4& operator+= (const Float4 & value);

	Float4 operator+ (const Float4& value) const;

	Float4 operator+ (float value) const;

	Float4& operator-= (const Float4& value);

	Float4 operator- (const Float4& value) const;

	Float4 operator- (float value) const;

	Float4 operator-() const;

	Float4 operator*(float value) const;

	Float4 operator*(const Float4& value) const;

	bool operator==(const Float4& value) const;

	Float4 Normalize() const;

	float HalfX() const;

	float HalfY() const;

	float Size() const;
};

typedef Float4 Vector;
typedef Float4 Color;
