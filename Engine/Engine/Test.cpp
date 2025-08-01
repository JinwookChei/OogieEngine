#include "stdafx.h"
#include "Test.h"

bool CreateSphere(std::vector<SimpleVertex>* outVertices, std::vector<WORD>* outIndices, float radius /*= 0.5f*/)
{
	if (nullptr == outVertices || nullptr == outIndices)
	{
		return false;
	}

	const int SPHERE_STACKS = 20;
	const int SPHERE_SLICES = 20;
	const float PI = 3.14159265359f;

	for (int stack = 0; stack <= SPHERE_STACKS; ++stack)
	{
		float phi = PI * stack / SPHERE_STACKS; // 0 ~ PI
		float z = cosf(phi);
		float r = sinf(phi);

		for (int slice = 0; slice <= SPHERE_SLICES; ++slice)
		{
			float theta = 2.0f * PI * slice / SPHERE_SLICES; // 0 ~ 2PI
			float x = r * cosf(theta);
			float y = r * sinf(theta);

			DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(x * radius, y * radius, z * radius);
			DirectX::XMFLOAT4 normal = DirectX::XMFLOAT4(x, y, z, 0.0f);
			DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(
				0.5f + 0.5f * x,
				0.5f + 0.5f * y,
				0.5f + 0.5f * z,
				1.0f
			);

			float u = (float)slice / SPHERE_SLICES;
			float v = (float)stack / SPHERE_STACKS;

			DirectX::XMFLOAT2 uv = DirectX::XMFLOAT2(u, v);
			outVertices->push_back({ pos, color, normal, uv });
		}
	}

	for (int stack = 0; stack < SPHERE_STACKS; ++stack)
	{
		for (int slice = 0; slice < SPHERE_SLICES; ++slice)
		{
			int first = (stack * (SPHERE_SLICES + 1)) + slice;
			int second = first + SPHERE_SLICES + 1;

			// »ï°¢Çü 1
			outIndices->push_back((WORD)first);
			outIndices->push_back((WORD)second);
			outIndices->push_back((WORD)(first + 1));

			// »ï°¢Çü 2
			outIndices->push_back((WORD)second);
			outIndices->push_back((WORD)(second + 1));
			outIndices->push_back((WORD)(first + 1));
		}
	}

	std::vector<DirectX::XMFLOAT3> tan1(outVertices->size(), DirectX::XMFLOAT3(0, 0, 0));
	std::vector<DirectX::XMFLOAT3> tan2(outVertices->size(), DirectX::XMFLOAT3(0, 0, 0));

	for (size_t n = 0; n < outIndices->size(); n += 3)
	{
		WORD i0 = (*outIndices)[n];
		WORD i1 = (*outIndices)[n + 1];
		WORD i2 = (*outIndices)[n + 2];

		const Vector& p0 = (*outVertices)[i0].position;
		const Vector& p1 = (*outVertices)[i1].position;
		const Vector& p2 = (*outVertices)[i2].position;

		const Vector& uv0 = (*outVertices)[i0].UV;
		const Vector& uv1 = (*outVertices)[i1].UV;
		const Vector& uv2 = (*outVertices)[i2].UV;

		DirectX::XMVECTOR v0 = p0.dxVector;
		DirectX::XMVECTOR v1 = p1.dxVector;
		DirectX::XMVECTOR v2 = p2.dxVector;

		DirectX::XMVECTOR uvv0 = uv0.dxVector;
		DirectX::XMVECTOR uvv1 = uv1.dxVector;
		DirectX::XMVECTOR uvv2 = uv2.dxVector;

		float x1 = p1.X - p0.X;
		float y1 = p1.Y - p0.Y;
		float z1 = p1.Z - p0.Z;

		float x2 = p2.X - p0.X;
		float y2 = p2.Y - p0.Y;
		float z2 = p2.Z - p0.Z;

		float s1 = uv1.X - uv0.X;
		float t1 = uv1.Y - uv0.Y;

		float s2 = uv2.X - uv0.X;
		float t2 = uv2.Y - uv0.Y;

		float r = (s1 * t2 - s2 * t1);
		if (fabs(r) < 1e-6f)
		{
			r = 1.0f;
		}

		float invR = 1.0f / r;

		DirectX::XMFLOAT3 sdir(
			(t2 * x1 - t1 * x2) * invR,
			(t2 * y1 - t1 * y2) * invR,
			(t2 * z1 - t1 * z2) * invR
		);

		DirectX::XMFLOAT3 tdir(
			(s1 * x2 - s2 * x1) * invR,
			(s1 * y2 - s2 * y1) * invR,
			(s1 * z2 - s2 * z1) * invR
		);

		tan1[i0].x += sdir.x;
		tan1[i0].y += sdir.y;
		tan1[i0].z += sdir.z;

		tan1[i1].x += sdir.x;
		tan1[i1].y += sdir.y;
		tan1[i1].z += sdir.z;

		tan1[i2].x += sdir.x;
		tan1[i2].y += sdir.y;
		tan1[i2].z += sdir.z;

		tan2[i0].x += tdir.x;
		tan2[i0].y += tdir.y;
		tan2[i0].z += tdir.z;

		tan2[i1].x += tdir.x;
		tan2[i1].y += tdir.y;
		tan2[i1].z += tdir.z;

		tan2[i2].x += tdir.x;
		tan2[i2].y += tdir.y;
		tan2[i2].z += tdir.z;
	}

	for (size_t n = 0; n < outVertices->size(); ++n)
	{
		const Vector& normal = (*outVertices)[n].normal;
		const DirectX::XMFLOAT3& tangent = tan1[n];

		DirectX::XMVECTOR convertN = normal.dxVector;
		DirectX::XMVECTOR convertT = DirectX::XMLoadFloat3(&tangent);

		DirectX::XMVECTOR finalTangent = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(convertT, DirectX::XMVectorScale(convertN, DirectX::XMVectorGetX(DirectX::XMVector3Dot(convertN, convertT)))));

		DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&tan2[n]);
		float handedness = (DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVector3Cross(convertN, convertT), B)) < 0.0f) ? -1.0f : 1.0f;

		DirectX::XMFLOAT3 tangent3;
		DirectX::XMStoreFloat3(&tangent3, finalTangent);
		(*outVertices)[n].tangent = DirectX::XMFLOAT4(tangent3.x, tangent3.y, tangent3.z, handedness);

	}
	return true;
}