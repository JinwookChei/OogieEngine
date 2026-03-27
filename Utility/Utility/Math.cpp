#include "stdafx.h"
#include <xmmintrin.h>   // SSE
#include <smmintrin.h>   // SSE4.1 (있으면 더 좋음)
#include "Math.h"

static __m128 LoadFloat3(const Float3& value)
{
	__m128 xy = _mm_castpd_ps(_mm_load_sd((const double*)&value));
	__m128 z = _mm_load_ss(&value.Z);
	return _mm_movelh_ps(xy, z);
}

static void StoreFloat3(const __m128& value, Float3& dest)
{
	_mm_store_sd((double*)&dest, _mm_castps_pd(value));
	__m128 z = XM_PERMUTE_PS(value, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_store_ss(&dest.Z, z);
}


void MATH::VectorDot(float& out, const Float4& lhs, const Float4& rhs)
{
	__m128 result = DirectX::XMVector3Dot(_mm_loadu_ps(&lhs.X), _mm_loadu_ps(&rhs.X));
	out = result.m128_f32[0];
}


void MATH::VectorCross(Float4& out, const Float4& lhs, const Float4& rhs)
{
	__m128 result = DirectX::XMVector3Cross(_mm_loadu_ps(&lhs.X), _mm_loadu_ps(&rhs.X));
	_mm_storeu_ps(&out.X, result);
}

void MATH::VectorAdd(Float4& out, const Float4& lhs, const Float4& rhs)
{
	__m128 result = _mm_add_ps(_mm_loadu_ps(&lhs.X), _mm_loadu_ps(&rhs.X));
	_mm_storeu_ps(&out.X, result);
	if (0 != out.W)
	{
		out.W = 1.0f;
	}
}

void MATH::VectorSub(Float4& out, const Float4& lhs, const Float4& rhs)
{
	__m128 result = _mm_sub_ps(_mm_loadu_ps(&lhs.X), _mm_loadu_ps(&rhs.X));
	_mm_storeu_ps(&out.X, result);
	if (lhs.W != 0)
	{
		out.W = 1.0f;
	}
}

void MATH::VectorMultiply(Float4& out, const Float4& lhs, const Float4& rhs)
{
	__m128 result = _mm_mul_ps(_mm_loadu_ps(&lhs.X), _mm_loadu_ps(&rhs.X));
	_mm_storeu_ps(&out.X, result);
	if (0 != out.W)
	{
		out.W = 1.0f;
	}
}

void MATH::VectorScale(Float4& out, const Float4& lhs, float scale)
{
	__m128 vResult = _mm_set_ps1(scale);
	__m128 result = _mm_mul_ps(vResult, _mm_loadu_ps(&lhs.X));
	_mm_storeu_ps(&out.X, result);
	if (0 != out.W)
	{
		out.W = 1.0f;
	}
}

void MATH::VectorNormalize(Float4& out, const Float4& lhs)
{
	__m128 result = DirectX::XMVector3Normalize(_mm_loadu_ps(&lhs.X));
	_mm_storeu_ps(&out.X, result);
}

void MATH::VectorLength(float& out, const Float4& lhs)
{
	__m128 result = DirectX::XMVector3Length(_mm_loadu_ps(&lhs.X));
	out = result.m128_f32[0];
}

inline Float4 MATH::VectorSlerp(const Float4& q1, const Float4& q2, double weight)
{
	const float t = static_cast<float>(weight);

	// Load into SIMD registers
	__m128 v1 = _mm_loadu_ps(&q1.X);
	__m128 v2 = _mm_loadu_ps(&q2.X);

	// dot = dot(q1, q2)
	__m128 mul = _mm_mul_ps(v1, v2);

	// horizontal add
	__m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
	__m128 sums = _mm_add_ps(mul, shuf);

	shuf = _mm_shuffle_ps(sums, sums, _MM_SHUFFLE(1, 0, 3, 2));
	sums = _mm_add_ps(sums, shuf);

	float dot = _mm_cvtss_f32(sums);

	// If dot < 0, negate second quaternion
	if (dot < 0.0f)
	{
		dot = -dot;
		v2 = _mm_sub_ps(_mm_setzero_ps(), v2);
	}

	const float DOT_THRESHOLD = 0.9995f;

	// If very close -> use Lerp
	if (dot > DOT_THRESHOLD)
	{
		// result = normalize(q1 + t*(q2-q1))

		__m128 diff = _mm_sub_ps(v2, v1);
		__m128 tv = _mm_set1_ps(t);

		__m128 result = _mm_add_ps(v1, _mm_mul_ps(diff, tv));

		// normalize
		__m128 sq = _mm_mul_ps(result, result);

		shuf = _mm_shuffle_ps(sq, sq, _MM_SHUFFLE(2, 3, 0, 1));
		sums = _mm_add_ps(sq, shuf);

		shuf = _mm_shuffle_ps(sums, sums, _MM_SHUFFLE(1, 0, 3, 2));
		sums = _mm_add_ps(sums, shuf);

		float len = std::sqrt(_mm_cvtss_f32(sums));

		__m128 invLen = _mm_set1_ps(1.0f / len);
		result = _mm_mul_ps(result, invLen);

		Float4 out;
		_mm_storeu_ps(&out.X, result);
		return out;
	}

	// Standard Slerp
	float theta = std::acos(dot);
	float sinTheta = std::sin(theta);

	float w1 = std::sin((1.0f - t) * theta) / sinTheta;
	float w2 = std::sin(t * theta) / sinTheta;

	__m128 vw1 = _mm_set1_ps(w1);
	__m128 vw2 = _mm_set1_ps(w2);

	__m128 r1 = _mm_mul_ps(v1, vw1);
	__m128 r2 = _mm_mul_ps(v2, vw2);

	__m128 result = _mm_add_ps(r1, r2);

	Float4 out;
	_mm_storeu_ps(&out.X, result);
	return out;
}

void MATH::VectorDot(float& out, const Float3& lhs, const Float3& rhs)
{
	__m128 result = DirectX::XMVector3Dot(LoadFloat3(lhs), LoadFloat3(rhs));
	out = result.m128_f32[0];
}

void MATH::VectorCross(Float3& out, const Float3& lhs, const Float3& rhs)
{
	__m128 result = DirectX::XMVector3Cross(LoadFloat3(lhs), LoadFloat3(rhs));
	StoreFloat3(result, out);
}

void MATH::VectorAdd(Float3& out, const Float3& lhs, const Float3& rhs)
{
	__m128 result = _mm_add_ps(LoadFloat3(lhs), LoadFloat3(rhs));
	StoreFloat3(result, out);
}

void MATH::VectorSub(Float3& out, const Float3& lhs, const Float3& rhs)
{
	__m128 result = _mm_sub_ps(LoadFloat3(lhs), LoadFloat3(rhs));
	StoreFloat3(result, out);
}

void MATH::VectorMultiply(Float3& out, const Float3& lhs, const Float3& rhs)
{
	__m128 result = _mm_mul_ps(LoadFloat3(lhs), LoadFloat3(rhs));
	StoreFloat3(result, out);
}

void MATH::VectorScale(Float3& out, const Float3& lhs, float scale)
{
	__m128 vResult = _mm_set_ps1(scale);
	__m128 result = _mm_mul_ps(vResult, LoadFloat3(lhs));
	StoreFloat3(result, out);
}

void MATH::VectorNormalize(Float3& out, const Float3& lhs)
{
	__m128 result = DirectX::XMVector3Normalize(LoadFloat3(lhs));
	StoreFloat3(result, out);
}

void MATH::VectorLength(float& out, const Float3& lhs)
{
	__m128 result = DirectX::XMVector3Length(LoadFloat3(lhs));
	out = result.m128_f32[0];
}

void MATH::VectorToEulerDeg(Float4& out, const Float3& vector)
{
	// 1. DirectXMath로 벡터 생성 및 정규화
	Float3 v;
	VectorNormalize(v, vector);
	float nx = v.X;
	float ny = v.Y;
	float nz = v.Z;

	// LHS, X-front, Y-right, Z-up
	// 2. Roll (Y-axis rotation) -> 단일 벡터로는 결정 불가
	out.X = 0.0f;

	// 3. Pitch (X-axis rotation)
	out.Y = ConvertRadToDeg(atan2f(ny, nx)); // Yaw: rotation in X-Y plane

	// 4. Yaw (Z-axis rotation)
	out.Z = ConvertRadToDeg(asinf(-nz)); // Pitch: positive -> nose up

	// 5. W는 사용하지 않거나 1로 설정
	out.W = 1.0f;
}


void MATH::VectorDot(float& out, const Float2& lhs, const Float2& rhs)
{
	// 아직 검증 안됨. 사용 할 일 있으면 검증 해서 사용.
	// 디버그 봉인 씰
	DEBUG_BREAK();

	// [ x, y, 0, 0 ]
	__m128 a = _mm_set_ps(0.0f, 0.0f, lhs.Y, lhs.X);
	__m128 b = _mm_set_ps(0.0f, 0.0f, rhs.Y, rhs.X);

#if defined(__SSE4_1__)
	// SSE4.1 전용: dot 명령어 한 방
	__m128 dot = _mm_dp_ps(a, b, 0x31); // x,y만 곱하고 x에 결과
	out = _mm_cvtss_f32(dot);
#else
	// SSE2 호환 버전
	__m128 mul = _mm_mul_ps(a, b);      // [x*x, y*y, 0, 0]
	__m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
	__m128 sum = _mm_add_ss(mul, shuf); // x*x + y*y
	out = _mm_cvtss_f32(sum);
#endif
}

//void MATH::VectorCross(Float2& out, const Float2& lhs, const Float2& rhs)
//{
//}

void MATH::VectorAdd(Float2& out, const Float2& lhs, const Float2& rhs)
{
	// 아직 검증 안됨. 사용 할 일 있으면 검증 해서 사용.
	// 디버그 봉인 씰
	DEBUG_BREAK();

	// [ x, y, 0, 0 ]
	__m128 a = _mm_set_ps(0.0f, 0.0f, lhs.Y, lhs.X);
	__m128 b = _mm_set_ps(0.0f, 0.0f, rhs.Y, rhs.X)	;

	__m128 sum = _mm_add_ps(a, b);

	out.X = _mm_cvtss_f32(sum);
	out.Y = _mm_cvtss_f32(_mm_shuffle_ps(sum, sum, _MM_SHUFFLE(3, 2, 1, 1)));
}

void MATH::VectorSub(Float2& out, const Float2& lhs, const Float2& rhs)
{
	// 아직 검증 안됨. 사용 할 일 있으면 검증 해서 사용.
	// 디버그 봉인 씰
	DEBUG_BREAK();

	// [ x, y, 0, 0 ]
	__m128 a = _mm_set_ps(0.0f, 0.0f, lhs.Y, lhs.X);
	__m128 b = _mm_set_ps(0.0f, 0.0f, rhs.Y, rhs.X);

	__m128 diff = _mm_sub_ps(a, b);

	out.X = _mm_cvtss_f32(diff);
	out.Y = _mm_cvtss_f32(_mm_shuffle_ps(diff, diff, _MM_SHUFFLE(3, 2, 1, 1)));
}

void MATH::VectorMultiply(Float2& out, const Float2& lhs, const Float2& rhs)
{
	// 아직 검증 안됨. 사용 할 일 있으면 검증 해서 사용.
	// 디버그 봉인 씰
	DEBUG_BREAK();

	// [ x, y, 0, 0 ]
	__m128 a = _mm_set_ps(0.0f, 0.0f, lhs.Y, lhs.X);
	__m128 b = _mm_set_ps(0.0f, 0.0f, rhs.Y, rhs.X);

	__m128 mul = _mm_mul_ps(a, b);

	out.X = _mm_cvtss_f32(mul);
	out.Y = _mm_cvtss_f32(_mm_shuffle_ps(mul, mul, _MM_SHUFFLE(3, 2, 1, 1)));
}

void MATH::VectorScale(Float2& out, const Float2& lhs, float scale)
{
	// 아직 검증 안됨. 사용 할 일 있으면 검증 해서 사용.
	// 디버그 봉인 씰
	DEBUG_BREAK();

	// [ x, y, 0, 0 ]
	__m128 v = _mm_set_ps(0.0f, 0.0f, lhs.Y, lhs.X);

	// [ scale, scale, scale, scale ]
	__m128 s = _mm_set1_ps(scale);

	__m128 mul = _mm_mul_ps(v, s);

	out.X = _mm_cvtss_f32(mul);
	out.Y = _mm_cvtss_f32(_mm_shuffle_ps(mul, mul, _MM_SHUFFLE(3, 2, 1, 1)));
}

void MATH::VectorNormalize(Float2& out, const Float2& lhs)
{
	// 아직 검증 안됨. 사용 할 일 있으면 검증 해서 사용.
	// 디버그 봉인 씰
	DEBUG_BREAK();

	// [ x, y, 0, 0 ]
	__m128 v = _mm_set_ps(0.0f, 0.0f, lhs.Y, lhs.X);

	// length^2 = x*x + y*y
	__m128 mul = _mm_mul_ps(v, v); // [x^2, y^2, 0, 0]
	__m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(3, 2, 0, 1));
	__m128 lenSq = _mm_add_ss(mul, shuf);

	// length = sqrt(length^2)
	__m128 len = _mm_sqrt_ss(lenSq);

	// 0 벡터 체크
	float length = _mm_cvtss_f32(len);
	if (length <= 0.000001f)
	{
		out.X = 0.0f;
		out.Y = 0.0f;
		return;
	}

	// v / length
	__m128 invLen = _mm_div_ss(_mm_set_ss(1.0f), len);
	invLen = _mm_shuffle_ps(invLen, invLen, _MM_SHUFFLE(0, 0, 0, 0));

	__m128 norm = _mm_mul_ps(v, invLen);

	out.X = _mm_cvtss_f32(norm);
	out.Y = _mm_cvtss_f32(_mm_shuffle_ps(norm, norm, _MM_SHUFFLE(3, 2, 1, 1)));
}

void MATH::VectorLength(float& out, const Float2& lhs)
{
	// 아직 검증 안됨. 사용 할 일 있으면 검증 해서 사용.
	// 디버그 봉인 씰
	DEBUG_BREAK();

	// [ x, y, 0, 0 ]
	__m128 v = _mm_set_ps(0.0f, 0.0f, lhs.Y, lhs.X);

	// x^2, y^2
	__m128 mul = _mm_mul_ps(v, v); // [x^2, y^2, 0, 0]

	// x^2 + y^2
	__m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(3, 2, 0, 1));
	__m128 lenSq = _mm_add_ss(mul, shuf);

	// sqrt
	__m128 len = _mm_sqrt_ss(lenSq);

	out = _mm_cvtss_f32(len);
}


void MATH::QuaternionToEulerDeg(Float4& out, const Float4& quat)
{
	Float4 rad;
	QuaternionToEulerRad(rad, quat);
	VectorMultiply(out, rad, Float4(MATH::RadToDegFloat, MATH::RadToDegFloat, MATH::RadToDegFloat, 1.0f));
}

void MATH::QuaternionToEulerRad(Float4& out, const Float4& quat)
{
	// Roll (X축)
	float sinR_cosP = 2.0f * (quat.W * quat.X + quat.Y * quat.Z);
	float cosR_cosP = 1.0f - 2.0f * (quat.X * quat.X + quat.Y * quat.Y);
	out.X = atan2f(sinR_cosP, cosR_cosP);

	// Pitch (Y축)
	float sinP = 2.0f * (quat.W * quat.Y - quat.Z * quat.X);
	if (fabsf(sinP) >= 1.0f)
	{
		out.Y = copysignf(DirectX::XM_PIDIV2, sinP);
	}
	else
	{
		out.Y = asinf(sinP);
	}

	// Yaw (Z축)
	float sinY_cosP = 2.0f * (quat.W * quat.Z + quat.X * quat.Y);
	float cosY_cosP = 1.0f - 2.0f * (quat.Y * quat.Y + quat.Z * quat.Z);
	out.Z = atan2f(sinY_cosP, cosY_cosP);
}

void MATH::EulerDegToQuaternion(Float4& out, const Float4& deg)
{
	// Degree → radian
	DirectX::XMVECTOR rotRad = _mm_mul_ps(_mm_loadu_ps(&deg.X), _mm_loadu_ps(&MATH::DegToRad.X));
	// X-forward, Y-right, Z-up 기준 회전 구성
	DirectX::XMVECTOR qx = DirectX::XMQuaternionRotationAxis(DirectX::g_XMIdentityR0, DirectX::XMVectorGetX(rotRad)); // X (forward)
	DirectX::XMVECTOR qy = DirectX::XMQuaternionRotationAxis(DirectX::g_XMIdentityR1, DirectX::XMVectorGetY(rotRad)); // Y (right)
	DirectX::XMVECTOR qz = DirectX::XMQuaternionRotationAxis(DirectX::g_XMIdentityR2, DirectX::XMVectorGetZ(rotRad)); // Z (up)
	// 순서: Roll → Pitch → Yaw (필요에 맞게 조정)
	DirectX::XMVECTOR q = DirectX::XMQuaternionMultiply(qy, DirectX::XMQuaternionMultiply(qx, qz));

	DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(&out), q);
}


void MATH::CreateMatrixFromRows(Float3x3& out, const Float3& row0, const Float3& row1, const Float3& row2)
{
	__m128 r0 = _mm_set_ps(0.0f, row0.Z, row0.Y, row0.X);
	__m128 r1 = _mm_set_ps(0.0f, row1.Z, row1.Y, row1.X);
	__m128 r2 = _mm_set_ps(0.0f, row2.Z, row2.Y, row2.X);

	_mm_store_ps(&out.r[0].X, r0);
	_mm_store_ps(&out.r[1].X, r1);
	_mm_store_ps(&out.r[2].X, r2);
}
void MATH::CreateMatrixFromCols(Float3x3& out, const Float3& col0, const Float3& col1, const Float3& col2)
{
	__m128 r0 = _mm_set_ps(0.0f, col2.X, col1.X, col0.X);
	__m128 r1 = _mm_set_ps(0.0f, col2.Y, col1.Y, col0.Y);
	__m128 r2 = _mm_set_ps(0.0f, col2.Z, col1.Z, col0.Z);

	_mm_store_ps(&out.r[0].X, r0);
	_mm_store_ps(&out.r[1].X, r1);
	_mm_store_ps(&out.r[2].X, r2);
}
void MATH::CreateMatrixFromRows(Float4x4& out, const Float4& row0, const Float4& row1, const Float4& row2, const Float4& row3)
{
	__m128 r0 = _mm_set_ps(row0.W, row0.Z, row0.Y, row0.X);
	__m128 r1 = _mm_set_ps(row1.W, row1.Z, row1.Y, row1.X);
	__m128 r2 = _mm_set_ps(row2.W, row2.Z, row2.Y, row2.X);
	__m128 r3 = _mm_set_ps(row3.W, row3.Z, row3.Y, row3.X);

	_mm_store_ps(&out.r[0].X, r0);
	_mm_store_ps(&out.r[1].X, r1);
	_mm_store_ps(&out.r[2].X, r2);
	_mm_store_ps(&out.r[3].X, r3);
}
void MATH::CreateMatrixFromCols(Float4x4& out, const Float4& col0, const Float4& col1, const Float4& col2, const Float4& col3)
{
	__m128 r0 = _mm_set_ps(col3.X, col2.X, col1.X, col0.X); // 첫 행
	__m128 r1 = _mm_set_ps(col3.Y, col2.Y, col1.Y, col0.Y); // 두 번째 행
	__m128 r2 = _mm_set_ps(col3.Z, col2.Z, col1.Z, col0.Z); // 세 번째 행
	__m128 r3 = _mm_set_ps(col3.W, col2.W, col1.W, col0.W); // 마지막 row W는 패딩

	_mm_store_ps(&out.r[0].X, r0);
	_mm_store_ps(&out.r[1].X, r1);
	_mm_store_ps(&out.r[2].X, r2);
	_mm_store_ps(&out.r[3].X, r3);
}

void MATH::MatrixIdentity(Float4x4& out)
{
	DirectX::XMMATRIX matrix = DirectX::XMMatrixIdentity();
	memcpy_s(&out, sizeof(Float4x4), &matrix, sizeof(DirectX::XMMATRIX));
}

void MATH::MatrixMultiply(Float4& out, const Float4x4& lhs, const Float4& rhs)
{
	// Load vector
	__m128 v = _mm_load_ps(&rhs.X); // [x y z w]

	// row0
	__m128 r0 = _mm_load_ps(&lhs.r[0].X);
	__m128 o0 = _mm_dp_ps(r0, v, 0xF1);  // dot(row0, v)
	out.X = _mm_cvtss_f32(o0);

	// row1
	__m128 r1 = _mm_load_ps(&lhs.r[1].X);
	__m128 o1 = _mm_dp_ps(r1, v, 0xF1);
	out.Y = _mm_cvtss_f32(o1);

	// row2
	__m128 r2 = _mm_load_ps(&lhs.r[2].X);
	__m128 o2 = _mm_dp_ps(r2, v, 0xF1);
	out.Z = _mm_cvtss_f32(o2);

	// row3
	__m128 r3 = _mm_load_ps(&lhs.r[3].X);
	__m128 o3 = _mm_dp_ps(r3, v, 0xF1);
	out.W = _mm_cvtss_f32(o3);
}

void MATH::MatrixMultiply(Float4& out, const Float4& lhs, const Float4x4& rhs)
{
	// lhs 벡터를 로드 
	__m128 v = _mm_load_ps(&lhs.X);
	// [X Y Z W] 결과를 초기화 
	__m128 result = _mm_setzero_ps();
	// 각 성분에 대해 행렬 열을 곱하고 합산 
	__m128 col0 = _mm_set1_ps(lhs.X);
	__m128 col1 = _mm_set1_ps(lhs.Y);
	__m128 col2 = _mm_set1_ps(lhs.Z);
	__m128 col3 = _mm_set1_ps(lhs.W);
	// 행렬 열별 곱셈 
	__m128 r0 = _mm_load_ps(&rhs.r[0].X);
	__m128 r1 = _mm_load_ps(&rhs.r[1].X);
	__m128 r2 = _mm_load_ps(&rhs.r[2].X);
	__m128 r3 = _mm_load_ps(&rhs.r[3].X);
	result = _mm_add_ps(result, _mm_mul_ps(col0, r0));
	result = _mm_add_ps(result, _mm_mul_ps(col1, r1));
	result = _mm_add_ps(result, _mm_mul_ps(col2, r2));
	result = _mm_add_ps(result, _mm_mul_ps(col3, r3));
	// 결과를 out에 저장 
	_mm_store_ps(&out.X, result);
}

void MATH::MatrixMultiply(Float4x4& out, const Float4x4& lhs, const Float4x4& rhs)
{
	// We'll read rhs columns by loading each row then extracting columns via shuffle,
	// but an efficient approach is to transpose rhs into columns or compute with broadcasts.

	// Pre-load rhs rows
	__m128 r0 = _mm_load_ps(&rhs.r[0].X);
	__m128 r1 = _mm_load_ps(&rhs.r[1].X);
	__m128 r2 = _mm_load_ps(&rhs.r[2].X);
	__m128 r3 = _mm_load_ps(&rhs.r[3].X);

	// For each lhs row, compute dot with each column of rhs.
	for (int i = 0; i < 4; ++i) {
		__m128 L = _mm_load_ps(&lhs.r[i].X); // lhs row i

		// Broadcast L.x
		__m128 lx = _mm_shuffle_ps(L, L, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 ly = _mm_shuffle_ps(L, L, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 lz = _mm_shuffle_ps(L, L, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 lw = _mm_shuffle_ps(L, L, _MM_SHUFFLE(3, 3, 3, 3));

		// Multiply broadcasted components with rhs rows, then sum to get row result
		__m128 t0 = _mm_mul_ps(lx, r0);
		__m128 t1 = _mm_mul_ps(ly, r1);
		__m128 t2 = _mm_mul_ps(lz, r2);
		__m128 t3 = _mm_mul_ps(lw, r3);

		__m128 sum01 = _mm_add_ps(t0, t1);
		__m128 sum23 = _mm_add_ps(t2, t3);
		__m128 rowResult = _mm_add_ps(sum01, sum23);

		_mm_store_ps(&out.r[i].X, rowResult);
	}
}

void MATH::MatrixTranspose(Float4x4& out, const Float4x4& src)
{
	DirectX::XMMATRIX srcMatrix;
	memcpy_s(&srcMatrix, sizeof(DirectX::XMMATRIX), &src, sizeof(Float4x4));

	DirectX::XMMATRIX matrix = DirectX::XMMatrixTranspose(srcMatrix);

	memcpy_s(&out, sizeof(Float4x4), &matrix, sizeof(DirectX::XMMATRIX));
}

void MATH::MatrixInverse(Float4x4& out, const Float4x4& src)
{
	DirectX::XMMATRIX srcMatrix;
	memcpy_s(&srcMatrix, sizeof(DirectX::XMMATRIX), &src, sizeof(Float4x4));

	DirectX::XMVECTOR det;
	DirectX::XMMATRIX matrix = DirectX::XMMatrixInverse(&det, srcMatrix);
	if (DirectX::XMVectorGetX(det) == 0.0f)
	{
		Assert("역행렬이 존재 하지 않습니다.");
		return;
	}

	memcpy_s(&out, sizeof(Float4x4), &matrix, sizeof(DirectX::XMMATRIX));
}

/// <summary>
/// 완전 SIMD를 활용한 함수는 아님. 가독성을 위해 간략히 계산. 성능이슈있으면 리팰토링 할 것.
/// </summary>
void MATH::MatrixDeterminant(float& out, const Float4x4& src)
{
	// 행을 SIMD로 한번에 로드 (정렬 불확실하면 _mm_loadu_ps)
	__m128 row0 = _mm_loadu_ps(&src.r[0].X); // [r0.x r0.y r0.z r0.w]
	__m128 row1 = _mm_loadu_ps(&src.r[1].X);
	__m128 row2 = _mm_loadu_ps(&src.r[2].X);
	__m128 row3 = _mm_loadu_ps(&src.r[3].X);

	// SIMD에서 각 성분을 추출 (간단하고 안전한 방법)
	float a00 = src.r[0].X, a01 = src.r[0].Y, a02 = src.r[0].Z, a03 = src.r[0].W;
	float a10 = src.r[1].X, a11 = src.r[1].Y, a12 = src.r[1].Z, a13 = src.r[1].W;
	float a20 = src.r[2].X, a21 = src.r[2].Y, a22 = src.r[2].Z, a23 = src.r[2].W;
	float a30 = src.r[3].X, a31 = src.r[3].Y, a32 = src.r[3].Z, a33 = src.r[3].W;

	// 3x3 소행렬식 계산 함수 (명확성 위해 인라인으로 작성)
	auto det3 = [](float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22) -> float
		{
			// det = m00*(m11*m22 - m12*m21) - m01*(m10*m22 - m12*m20) + m02*(m10*m21 - m11*m20)
			return m00 * (m11 * m22 - m12 * m21)
				- m01 * (m10 * m22 - m12 * m20)
				+ m02 * (m10 * m21 - m11 * m20);
		};

	// 첫 번째 행의 각 원소에 대응하는 3x3 소행렬의 행렬식 (첫 행 제거, 각 열 순서로 제외)
	// minor0 : remove col 0 -> use columns 1,2,3 from rows 1..3
	float m0 = det3(
		a11, a12, a13,
		a21, a22, a23,
		a31, a32, a33
	);

	// minor1 : remove col 1 -> use cols 0,2,3
	float m1 = det3(
		a10, a12, a13,
		a20, a22, a23,
		a30, a32, a33
	);

	// minor2 : remove col 2 -> use cols 0,1,3
	float m2 = det3(
		a10, a11, a13,
		a20, a21, a23,
		a30, a31, a33
	);

	// minor3 : remove col 3 -> use cols 0,1,2
	float m3 = det3(
		a10, a11, a12,
		a20, a21, a22,
		a30, a31, a32
	);

	// Laplace expansion along the first row:
	// det = a00 * m0 - a01 * m1 + a02 * m2 - a03 * m3
	out = a00 * m0 - a01 * m1 + a02 * m2 - a03 * m3;
}

void MATH::MatrixDeterminant(float& out, const Float3x3& src)
{
	// Load rows (XYZ, W는 패딩)
	__m128 r0 = _mm_load_ps(&src.r[0].X);
	__m128 r1 = _mm_load_ps(&src.r[1].X);
	__m128 r2 = _mm_load_ps(&src.r[2].X);

	// r1.yzxw  (shuffle to [y z x w])
	__m128 r1_yzx = _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 0, 2, 1));
	// r2.zxyw  (shuffle to [z x y w])
	__m128 r2_zxy = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(3, 1, 0, 2));
	// r1.zxyw  (shuffle to [z x y w])
	__m128 r1_zxy = _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 1, 0, 2));
	// r2.yzxw  (shuffle to [y z x w])
	__m128 r2_yzx = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(3, 0, 2, 1));

	// cross = r1 x r2 = (r1_yzx * r2_zxy) - (r1_zxy * r2_yzx)
	__m128 c0 = _mm_mul_ps(r1_yzx, r2_zxy);
	__m128 c1 = _mm_mul_ps(r1_zxy, r2_yzx);
	__m128 cross = _mm_sub_ps(c0, c1);

	// mul = r0 * cross -> [x*cx, y*cy, z*cz, w*cw]
	__m128 mul = _mm_mul_ps(r0, cross);

	// 안전하고 확실한 방법: 필요한 3요소(x,y,z)만 추출해서 합산
	float x = _mm_cvtss_f32(mul);                                    // element 0
	float y = _mm_cvtss_f32(_mm_shuffle_ps(mul, mul, _MM_SHUFFLE(3, 2, 1, 1))); // element 1
	float z = _mm_cvtss_f32(_mm_shuffle_ps(mul, mul, _MM_SHUFFLE(3, 2, 1, 2))); // element 2

	out = x + y + z;
}

void MATH::MatrixCompose(Float4x4& worldMat, const Float4& scale, const Float4& quat, const Float4& trans)
{
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.X, scale.Y, scale.Z);
	DirectX::XMVECTOR Q = DirectX::XMVectorSet(quat.X, quat.Y, quat.Z, quat.W);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(Q);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(trans.X, trans.Y, trans.Z);

	// World = S * R * T
	DirectX::XMMATRIX W = S * R * T;

	// Store into your Float4x4
	DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&worldMat), W);
}

void MATH::MatrixCompose(Float4x4& worldMat, Float4x4& scaleMat, Float4x4& rotMat, Float4x4& transMat, const Float4& scale, const Float4& quat, const Float4& trans)
{
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.X, scale.Y, scale.Z);
	DirectX::XMVECTOR Q = DirectX::XMVectorSet(quat.X, quat.Y, quat.Z, quat.W);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(Q);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(trans.X, trans.Y, trans.Z);

	// World = S * R * T
	DirectX::XMMATRIX W = S * R * T;

	// Store into your Float4x4
	DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&worldMat), W);
	DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&scaleMat), S);
	DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&rotMat), R);
	DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&transMat), T);
}

void MATH::MatrixDecompose(Float4& outScale, Float4& outQuat, Float4& outPos, const Float4x4& src)
{
	DirectX::XMMATRIX srcMatrix;
	memcpy_s(&srcMatrix, sizeof(DirectX::XMMATRIX), &src, sizeof(Float4x4));
	DirectX::XMVECTOR tmpScale;
	DirectX::XMVECTOR tmpQuat;
	DirectX::XMVECTOR tmpPos;
	DirectX::XMMatrixDecompose(&tmpScale, &tmpQuat, &tmpPos, srcMatrix);
	_mm_storeu_ps(&outScale.X, tmpScale);
	_mm_storeu_ps(&outQuat.X, tmpQuat);
	_mm_storeu_ps(&outPos.X, tmpPos);
}

void MATH::MatrixLookAtLH(Float4x4& out, const Float4& eyePos, const Float4& focusPos, const Float4& eyeUp)
{
	DirectX::XMMATRIX matrix = DirectX::XMMatrixLookAtLH(
		_mm_loadu_ps(&eyePos.X),
		_mm_loadu_ps(&focusPos.X),
		_mm_loadu_ps(&eyeUp.X));

	memcpy_s(&out, sizeof(Float4x4), &matrix, sizeof(DirectX::XMMATRIX));
}

void MATH::MatrixLookToLH(Float4x4& out, const Float4& eyePos, const Float4& eyeForward, const Float4& eyeUp)
{
	DirectX::XMMATRIX matrix = DirectX::XMMatrixLookToLH(_mm_loadu_ps(&eyePos.X), _mm_loadu_ps(&eyeForward.X), _mm_loadu_ps(&eyeUp.X));
	memcpy_s(&out, sizeof(Float4x4), &matrix, sizeof(DirectX::XMMATRIX));
}

void MATH::MatrixPerspectiveFovLH(Float4x4& out, float fovDegAngle, float aspectRatio, float nearZ, float farZ)
{
	DirectX::XMMATRIX matrix = DirectX::XMMatrixPerspectiveFovLH(fovDegAngle * MATH::DegToRadFloat, aspectRatio, nearZ, farZ);
	memcpy_s(&out, sizeof(Float4x4), &matrix, sizeof(DirectX::XMMATRIX));
}

float MATH::ConvertDegToRad(float deg)
{
	return deg * MATH::DegToRadFloat;
}

float MATH::ConvertRadToDeg(float rad)
{
	return rad * MATH::RadToDegFloat;
}


//bool MATH::Intersection3D_Ray_Triangle
//(
//	float* pOutT,
//	const Float3& rayPos, const Float3& rayDir,
//	const Float3& triA, const Float3& triB, const Float3& triC
//)
//{
//	Float3 AtoC;
//	VectorSub(AtoC, triC, triA);
//
//	Float3 BtoC;
//	VectorSub(BtoC, triC, triB);
//
//	Float3 StoC;
//	VectorSub(StoC, triC, rayPos);
//
//	Float3x3 matDenominator;
//	CreateMatrixFromCols(matDenominator, rayDir, AtoC, BtoC);
//	Float3x3 matT;
//	CreateMatrixFromCols(matT, StoC, AtoC, BtoC);
//	Float3x3 matU;
//	CreateMatrixFromCols(matU, rayDir, StoC, BtoC);
//	Float3x3 matV;
//	CreateMatrixFromCols(matV, rayDir, AtoC, StoC);
//
//
//	float detDenominator;
//	MatrixDeterminant(detDenominator, matDenominator);
//	float detT;
//	MatrixDeterminant(detT, matT);
//	float detU;
//	MatrixDeterminant(detU, matU);
//	float detV;
//	MatrixDeterminant(detV, matV);
//
//	float t = detT / detDenominator;
//	float u = detU / detDenominator;
//	float v = detV / detDenominator;
//
//	if (u >= 0 && v >= 0 && u + v <= 1)
//	{
//		*pOutT = t;
//		return true;
//	}
//
//	return false;
//}

bool MATH::Intersection3D_Ray_Triangle(float* pOutDistance, const Ray& ray, const Triangle& triangle)
{
	Float3 rayOrigin{ ray.origin_.X,ray.origin_.Y, ray.origin_.Z };
	Float3 rayDir{ ray.dir_.X ,ray.dir_.Y ,ray.dir_.Z };

	Float3 aToc;
	VectorSub(aToc, triangle.c_, triangle.a_);
	Float3 bToc;
	VectorSub(bToc, triangle.c_, triangle.b_);
	Float3 sToc;
	VectorSub(sToc, triangle.c_, rayOrigin);

	Float3x3 matDenominator;
	CreateMatrixFromCols(matDenominator, rayDir, aToc, bToc);
	Float3x3 matT;
	CreateMatrixFromCols(matT, sToc, aToc, bToc);
	Float3x3 matU;
	CreateMatrixFromCols(matU, rayDir, sToc, bToc);
	Float3x3 matV;
	CreateMatrixFromCols(matV, rayDir, aToc, sToc);

	float detDenominator;
	MatrixDeterminant(detDenominator, matDenominator);
	if (std::abs(detDenominator) < 1e-6f) return false;		// 0으로 나누는 상황 방지 (레이가 삼각형 평면과 평행할 때)

	float detT;
	MatrixDeterminant(detT, matT);
	float detU;
	MatrixDeterminant(detU, matU);
	float detV;
	MatrixDeterminant(detV, matV);

	float t = detT / detDenominator;
	float u = detU / detDenominator;
	float v = detV / detDenominator;

	if (t < 0.0f || t > ray.maxDistance_)
	{
		return false;
	}
	else
	{
		if (u >= 0.0f && v >= 0.0f && u + v <= 1.0f)
		{
			*pOutDistance = t;
			return true;
		}
	}

	return false;
}

bool MATH::Intersection3D_Ray_AABB(float* pOutDistance, const Ray& ray, const AABB& aabb)
{
	// X축 슬랩(평면)에 대한 진입/진출 t 값 계산
	float t1 = (aabb.minPos_.X - ray.origin_.X) / ray.dir_.X;
	float t2 = (aabb.maxPos_.X - ray.origin_.X) / ray.dir_.X;
	// tmin은 항상 작은 값, tmax는 항상 큰 값이 되도록 설정
	float tMin = min(t1, t2);
	float tMax = max(t1, t2);

	// Y축 슬랩 검사
	float t3 = (aabb.minPos_.Y - ray.origin_.Y) / ray.dir_.Y;
	float t4 = (aabb.maxPos_.Y - ray.origin_.Y) / ray.dir_.Y;
	tMin = max(tMin, min(t3, t4));
	tMax = min(tMax, max(t3, t4));

	// Z축 슬랩 검사
	float t5 = (aabb.minPos_.Z - ray.origin_.Z) / ray.dir_.Z;
	float t6 = (aabb.maxPos_.Z - ray.origin_.Z) / ray.dir_.Z;
	tMin = max(tMin, min(t5, t6));
	tMax = min(tMax, max(t5, t6));

	// 충돌 조건 확인
	// 1. tMax < tMin: 슬랩들의 교차 구간이 없음 (빗나감)
	// 2. tMax < 0: AABB가 레이의 뒤쪽에 있음
	if (tMax < tMin || tMax < 0.0f) {
		return false;
	}

	float hitT = (tMin < 0.0f) ? tMax : tMin;

	if (hitT > ray.maxDistance_)
	{
		return false;
	}

	*pOutDistance = hitT;
	return true;
}