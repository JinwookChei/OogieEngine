#include "stdafx.h"
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

float MATH::VectorLength(const Float4& lhs)
{
	__m128 result = DirectX::XMVector3Length(_mm_loadu_ps(&lhs.X));
	return result.m128_f32[0];
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

float MATH::VectorLength(const Float3& lhs)
{
	__m128 result = DirectX::XMVector3Length(LoadFloat3(lhs));
	return result.m128_f32[0];
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


//void VectorToEulerDeg(Float4& out, const Float3& lhs)
//{
//	Float3 worldUp = { 0.0f, 0.0f, 1.0f };
//	Float3 right;
//	VectorCross(right, lhs, worldUp);
//
//	Float3 up;
//	VectorCross(up, right, lhs);
//
//	Float4 zero = { 0.0f, 0.0f, 0.0f, 1.0f };
//	Float4 dir = { lhs.X, lhs.Y, lhs.Z, 1.0f };
//	Float4 dirUp = { up.X, up.Y, up.Z, 0.0f };
//
//	Float4x4 tmpMat;
//	MatrixLookToLH(tmpMat, zero, dir, dirUp);
//
//
//	Float4 rotQuat;
//	MatrixDecomposeFromRotQ(tmpMat, rotQuat);
//
//	Float4 rotDegree;
//	QuaternionToEulerDeg(rotDegree, rotQuat);
//
//	out = rotDegree;
//
//
//}

//void ForwardToEulerDeg(Float3& outEulerDeg, const Float3& forward)
//{
//	// 월드 Up 벡터 (Z-up)
//	const Float3 worldUp = { 0.0f, 0.0f, 1.0f };
//
//	// Right 벡터 = Forward × Up
//	Float3 right;
//	VectorCross(right, forward, worldUp);
//	VectorNormalize(right, right);
//
//	// Up 벡터 = Right × Forward
//	Float3 up;
//	VectorCross(up, right, forward);
//	VectorNormalize(up, up);
//
//	// Yaw = Z-up 기준 회전
//	float yaw = atan2f(forward.Y, forward.X);
//
//	// Pitch = Forward 벡터가 XY 평면에서 위/아래로 향하는 각도
//	float pitch = atan2f(-forward.Z, sqrtf(forward.X * forward.X + forward.Y * forward.Y));
//
//	// Roll = Right 벡터와 Up 벡터를 기반으로 계산
//	float roll = atan2f(right.Z, up.Z);
//
//	// rad → deg
//	outEulerDeg.X = DirectX::XMConvertToDegrees(roll);
//	outEulerDeg.Y = DirectX::XMConvertToDegrees(pitch);
//	outEulerDeg.Z = DirectX::XMConvertToDegrees(yaw);
//}

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

//void QuaternionToEulerRad(Float4& out, const Float4& Q)
//{
//	float sinrCosp = 2.0f * (Q.W * Q.Z + Q.X * Q.Y);
//	float cosrCosp = 1.0f - 2.0f * (Q.Z * Q.Z + Q.X * Q.X);
//	out.Z = atan2f(sinrCosp, cosrCosp);
//
//	float pitchTest = Q.W * Q.X - Q.Y * Q.Z;
//	float asinThreshold = 0.4999995f;
//	float sinp = 2.0f * pitchTest;
//
//	if (pitchTest < -asinThreshold)
//	{
//		out.X = -(0.5f * MATH::PI);
//	}
//	else if (pitchTest > asinThreshold)
//	{
//		out.X = (0.5f * MATH::PI);
//	}
//	else
//	{
//		out.X = asinf(sinp);
//	}
//
//	float sinyCosp = 2.0f * (Q.W * Q.Y + Q.X * Q.Z);
//	float cosyCosp = 1.0f - 2.0f * (Q.X * Q.X + Q.Y * Q.Y);
//	out.Y = atan2f(sinyCosp, cosyCosp);
//}

//void RotationToQuaternion(Float4& outQ, const Float4& rot)
//{
//	// DirectXMath 사용
//	__m128 quat = DirectX::XMQuaternionRotationRollPitchYaw(rot.X, rot.Y, rot.Z);
//	// 주의: XMQuaternionRotationRollPitchYaw의 매개변수 순서는 pitch, yaw, roll
//	
//	// Float4에 저장
//	_mm_storeu_ps(&outQ.X, quat);
//}

//void QuaternionRotaionRollPitchYaw(Float4& outQ, const Float4& angle)
//{
//	__m128 result = DirectX::XMQuaternionRotationRollPitchYawFromVector(_mm_loadu_ps(&angle.X));
//	_mm_storeu_ps(&outQ.X, result);
//}

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

//void MatrixCompose(Float4x4& out, const Float4& scale, const Float4& rotAngle, const Float4& pos)
//{
//	__m128 convertRotAngle = _mm_loadu_ps(&rotAngle.X);
//	convertRotAngle = _mm_shuffle_ps(convertRotAngle, convertRotAngle, _MM_SHUFFLE(3, 0, 2, 1));
//	__m128 quaternionVectorOrigin = DirectX::XMQuaternionRotationRollPitchYawFromVector(convertRotAngle);
//
//	__m128 rotationVector = _mm_mul_ps(convertRotAngle, _mm_loadu_ps(&MATH::DegToRad.X));
//	rotationVector = _mm_shuffle_ps(rotationVector, rotationVector, _MM_SHUFFLE(3, 0, 2, 1));
//	__m128 quaternionVector = DirectX::XMQuaternionRotationRollPitchYawFromVector(rotationVector);
//
//	DirectX::XMMATRIX matrix = DirectX::XMMatrixAffineTransformation(_mm_loadu_ps(&scale.X), quaternionVectorOrigin, quaternionVector, _mm_loadu_ps(&pos.X));
//	memcpy_s(&out, sizeof(Float4x4), &matrix, sizeof(DirectX::XMMATRIX));
//}

//void MatrixCompose(Float4x4& out, const Float4& scale, const Float4& rotAngleDeg, const Float4& pos)
//{
//	// degree → radian
//	__m128 radianAngles = _mm_mul_ps(_mm_loadu_ps(&rotAngleDeg.X), _mm_loadu_ps(&MATH::DegToRad.X));
//
//	radianAngles = _mm_shuffle_ps(radianAngles, radianAngles, _MM_SHUFFLE(3, 1, 2, 0));
//	
//	
//	// Pitch, Yaw, Roll 순서로 파라미터를 받음.
//	__m128 quaternion = DirectX::XMQuaternionRotationRollPitchYawFromVector(radianAngles);
//
//	// 원점 기준 affine 변환 행렬
//	DirectX::XMMATRIX matrix = DirectX::XMMatrixAffineTransformation(
//		_mm_loadu_ps(&scale.X),
//		DirectX::g_XMZero,   // 회전 중심 = 원점
//		quaternion,
//		_mm_loadu_ps(&pos.X));
//
//	memcpy_s(&out, sizeof(Float4x4), &matrix, sizeof(DirectX::XMMATRIX));
//}

void MATH::MatrixCompose(Float4x4& out, const Float4& scale, const Float4& rotDeg, const Float4& pos)
{
	using namespace DirectX;

	// Degree → radian
	XMVECTOR rotRad = _mm_mul_ps(_mm_loadu_ps(&rotDeg.X), _mm_loadu_ps(&MATH::DegToRad.X));

	// X-forward, Y-right, Z-up 기준 회전 구성
	XMVECTOR qx = XMQuaternionRotationAxis(g_XMIdentityR0, XMVectorGetX(rotRad)); // X (forward)
	XMVECTOR qy = XMQuaternionRotationAxis(g_XMIdentityR1, XMVectorGetY(rotRad)); // Y (right)
	XMVECTOR qz = XMQuaternionRotationAxis(g_XMIdentityR2, XMVectorGetZ(rotRad)); // Z (up)

	// 순서: Roll → Pitch → Yaw (필요에 맞게 조정)
	XMVECTOR q = XMQuaternionMultiply(qy, XMQuaternionMultiply(qx, qz));

	XMMATRIX matrix = XMMatrixAffineTransformation(
		_mm_loadu_ps(&scale.X),
		g_XMZero,
		q,
		_mm_loadu_ps(&pos.X));

	memcpy_s(&out, sizeof(Float4x4), &matrix, sizeof(XMMATRIX));
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

//void MatrixDecomposeFromRotQ(const Float4x4& matrx, Float4& rotQ)
//{
//	Float4 tmpScale;
//	Float4 tmpPos;
//	MatrixDecompose(matrx, tmpScale, rotQ, tmpPos);
//}

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