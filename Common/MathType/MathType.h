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
