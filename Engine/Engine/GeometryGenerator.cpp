#include "stdafx.h"
#include "GeometryGenerator.h"

bool GeometryGenerator::CreateSphere(std::vector<SimpleVertex>* outVertices, std::vector<WORD>* outIndices, float radius)
{
	if (nullptr == outVertices || nullptr == outIndices)
	{
		return false;
	}

	const int SPHERE_STACKS = 20;
	const int SPHERE_SLICES = 20;

	for (int stack = 0; stack <= SPHERE_STACKS; ++stack)
	{
		float phi = MATH::PI * stack / SPHERE_STACKS; // 0 ~ PI
		float z = cosf(phi);
		float r = sinf(phi);

		for (int slice = 0; slice <= SPHERE_SLICES; ++slice)
		{
			float theta = 2.0f * MATH::PI * slice / SPHERE_SLICES; // 0 ~ 2PI
			float x = r * cosf(theta);
			float y = r * sinf(theta);

			Float3 pos = Float3(x * radius, y * radius, z * radius);
			Float3 normal = Float3(x, y, z);
			Float4 color = Float4(
				0.5f + 0.5f * x,
				0.5f + 0.5f * y,
				0.5f + 0.5f * z,
				1.0f
			);

			float u = (float)slice / SPHERE_SLICES;
			float v = (float)stack / SPHERE_STACKS;

			Float2 uv = Float2(u, v);
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

	std::vector<Float3> tan1(outVertices->size(), Float3(0, 0, 0));
	std::vector<Float3> tan2(outVertices->size(), Float3(0, 0, 0));

	for (size_t n = 0; n < outIndices->size(); n += 3)
	{
		WORD i0 = (*outIndices)[n];
		WORD i1 = (*outIndices)[n + 1];
		WORD i2 = (*outIndices)[n + 2];

		const Float3& p0 = (*outVertices)[i0].position;
		const Float3& p1 = (*outVertices)[i1].position;
		const Float3& p2 = (*outVertices)[i2].position;

		const Float2& uv0 = (*outVertices)[i0].UV;
		const Float2& uv1 = (*outVertices)[i1].UV;
		const Float2& uv2 = (*outVertices)[i2].UV;

		//DirectX::XMVECTOR v0 = p0.dxVector;
		//DirectX::XMVECTOR v1 = p1.dxVector;
		//DirectX::XMVECTOR v2 = p2.dxVector;

		//DirectX::XMVECTOR uvv0 = uv0.dxVector;
		//DirectX::XMVECTOR uvv1 = uv1.dxVector;
		//DirectX::XMVECTOR uvv2 = uv2.dxVector;

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

		Float3 sdir(
			(t2 * x1 - t1 * x2) * invR,
			(t2 * y1 - t1 * y2) * invR,
			(t2 * z1 - t1 * z2) * invR
		);

		Float3 tdir(
			(s1 * x2 - s2 * x1) * invR,
			(s1 * y2 - s2 * y1) * invR,
			(s1 * z2 - s2 * z1) * invR
		);

		tan1[i0].X += sdir.X;
		tan1[i0].Y += sdir.Y;
		tan1[i0].Z += sdir.Z;

		tan1[i1].X += sdir.X;
		tan1[i1].Y += sdir.Y;
		tan1[i1].Z += sdir.Z;

		tan1[i2].X += sdir.X;
		tan1[i2].Y += sdir.Y;
		tan1[i2].Z += sdir.Z;

		tan2[i0].X += tdir.X;
		tan2[i0].Y += tdir.Y;
		tan2[i0].Z += tdir.Z;

		tan2[i1].X += tdir.X;
		tan2[i1].Y += tdir.Y;
		tan2[i1].Z += tdir.Z;

		tan2[i2].X += tdir.X;
		tan2[i2].Y += tdir.Y;
		tan2[i2].Z += tdir.Z;
	}

	for (size_t n = 0; n < outVertices->size(); ++n)
	{
		const Float3& normal = (*outVertices)[n].normal;
		const Float3& tangent = tan1[n];
		const Float3& tangent2 = tan2[n];

		float dotResult = 0.0f;
		MATH::VectorDot(dotResult, normal, tangent);

		Float3 scaleResult;
		MATH::VectorScale(scaleResult, normal, dotResult);

		Float3 subResult;
		MATH::VectorSub(subResult, tangent, scaleResult);

		Float3  finalTangent;
		MATH::VectorNormalize(finalTangent, subResult);

		Float3 crossResult;
		MATH::VectorCross(crossResult, normal, tangent);

		float dotResult2 = 0.0f;
		MATH::VectorDot(dotResult2, crossResult, tangent2);

		float handedness = (dotResult2 < 0.0f) ? -1.0f : 1.0f;

		(*outVertices)[n].tangent = Float4(finalTangent.X, finalTangent.Y, finalTangent.Z, handedness);

	}
	return true;
}

bool GeometryGenerator::CreateCube(std::vector<SimpleVertex>* outVertices, std::vector<WORD>* outIndices, float halfExtent)
{
	if (nullptr == outVertices || nullptr == outIndices) {
		return false;
	}
	
	float h = halfExtent;

	outVertices->clear();
	outVertices->reserve(24);
	
	// µÚ (-Z)
	outVertices->push_back(SimpleVertex(Float3(-h, -h, -h), Float4(1, 0, 0, 1), Float3(0, 0, -1), Float2(0, 1), Float4(-1, 0, 0, 1)));
	outVertices->push_back(SimpleVertex(Float3(-h, h, -h), Float4(1, 0, 0, 1), Float3(0, 0, -1), Float2(0, 0), Float4(-1, 0, 0, 1)));
	outVertices->push_back(SimpleVertex(Float3(h, h, -h), Float4(1, 0, 0, 1), Float3(0, 0, -1), Float2(1, 0), Float4(-1, 0, 0, 1)));
	outVertices->push_back(SimpleVertex(Float3(h, -h, -h), Float4(1, 0, 0, 1), Float3(0, 0, -1), Float2(1, 1), Float4(-1, 0, 0, 1)));

	// ¾Õ (+Z)
	outVertices->push_back(SimpleVertex(Float3(-h, -h, h), Float4(1, 0, 0, 1), Float3(0, 0, 1), Float2(1, 1), Float4(1, 0, 0, 1)));
	outVertices->push_back(SimpleVertex(Float3(h, -h, h), Float4(1, 0, 0, 1), Float3(0, 0, 1), Float2(0, 1), Float4(1, 0, 0, 1)));
	outVertices->push_back(SimpleVertex(Float3(h, h, h), Float4(1, 0, 0, 1), Float3(0, 0, 1), Float2(0, 0), Float4(1, 0, 0, 1)));
	outVertices->push_back(SimpleVertex(Float3(-h, h, h), Float4(1, 0, 0, 1), Float3(0, 0, 1), Float2(1, 0), Float4(1, 0, 0, 1)));

	// ¿ÞÂÊ (-X)
	outVertices->push_back(SimpleVertex(Float3(-h, -h, h), Float4(0, 1, 0, 1), Float3(-1, 0, 0), Float2(0, 1), Float4(0, 0, 1, 1)));
	outVertices->push_back(SimpleVertex(Float3(-h, h, h), Float4(0, 1, 0, 1), Float3(-1, 0, 0), Float2(0, 0), Float4(0, 0, 1, 1)));
	outVertices->push_back(SimpleVertex(Float3(-h, h, -h), Float4(0, 1, 0, 1), Float3(-1, 0, 0), Float2(1, 0), Float4(0, 0, 1, 1)));
	outVertices->push_back(SimpleVertex(Float3(-h, -h, -h), Float4(0, 1, 0, 1), Float3(-1, 0, 0), Float2(1, 1), Float4(0, 0, 1, 1)));

	// ¿À¸¥ÂÊ (+X)
	outVertices->push_back(SimpleVertex(Float3(h, -h, -h), Float4(0, 1, 0, 1), Float3(1, 0, 0), Float2(0, 1), Float4(0, 0, -1, 1)));
	outVertices->push_back(SimpleVertex(Float3(h, h, -h), Float4(0, 1, 0, 1), Float3(1, 0, 0), Float2(0, 0), Float4(0, 0, -1, 1)));
	outVertices->push_back(SimpleVertex(Float3(h, h, h), Float4(0, 1, 0, 1), Float3(1, 0, 0), Float2(1, 0), Float4(0, 0, -1, 1)));
	outVertices->push_back(SimpleVertex(Float3(h, -h, h), Float4(0, 1, 0, 1), Float3(1, 0, 0), Float2(1, 1), Float4(0, 0, -1, 1)));

	// À§ (+Y)
	outVertices->push_back(SimpleVertex(Float3(-h, h, -h), Float4(0, 0, 1, 1), Float3(0, 1, 0), Float2(0, 1), Float4(1, 0, 0, 1)));
	outVertices->push_back(SimpleVertex(Float3(-h, h, h), Float4(0, 0, 1, 1), Float3(0, 1, 0), Float2(0, 0), Float4(1, 0, 0, 1)));
	outVertices->push_back(SimpleVertex(Float3(h, h, h), Float4(0, 0, 1, 1), Float3(0, 1, 0), Float2(1, 0), Float4(1, 0, 0, 1)));
	outVertices->push_back(SimpleVertex(Float3(h, h, -h), Float4(0, 0, 1, 1), Float3(0, 1, 0), Float2(1, 1), Float4(1, 0, 0, 1)));

	// ¾Æ·¡ (-Y)
	outVertices->push_back(SimpleVertex(Float3(-h, -h, h), Float4(0, 0, 1, 1), Float3(0, -1, 0), Float2(1, 1), Float4(-1, 0, 1, 1)));
	outVertices->push_back(SimpleVertex(Float3(-h, -h, -h), Float4(0, 0, 1, 1), Float3(0, -1, 0), Float2(0, 1), Float4(-1, 0, 1, 1)));
	outVertices->push_back(SimpleVertex(Float3(h, -h, -h), Float4(0, 0, 1, 1), Float3(0, -1, 0), Float2(0, 0), Float4(-1, 0, 1, 1)));
	outVertices->push_back(SimpleVertex(Float3(h, -h, h), Float4(0, 0, 1, 1), Float3(0, -1, 0), Float2(1, 0), Float4(-1, 0, 1, 1)));



	outIndices->clear();
	outIndices->reserve(36);

	outIndices->push_back(0);
	outIndices->push_back(1);
	outIndices->push_back(2);

	outIndices->push_back(0);
	outIndices->push_back(2);
	outIndices->push_back(3);

	outIndices->push_back(4);
	outIndices->push_back(5);
	outIndices->push_back(6);

	outIndices->push_back(4);
	outIndices->push_back(6);
	outIndices->push_back(7);

	outIndices->push_back(8);
	outIndices->push_back(9);
	outIndices->push_back(10);

	outIndices->push_back(8);
	outIndices->push_back(10);
	outIndices->push_back(11);

	outIndices->push_back(12);
	outIndices->push_back(13);
	outIndices->push_back(14);

	outIndices->push_back(12);
	outIndices->push_back(14);
	outIndices->push_back(15);

	outIndices->push_back(16);
	outIndices->push_back(17);
	outIndices->push_back(18);

	outIndices->push_back(16);
	outIndices->push_back(18);
	outIndices->push_back(19);

	outIndices->push_back(20);
	outIndices->push_back(21);
	outIndices->push_back(22);

	outIndices->push_back(20);
	outIndices->push_back(22);
	outIndices->push_back(23);

	return true;
}

bool GeometryGenerator::CreateScreenRect(std::vector<ScreenRectVertex>* outVertices, std::vector<WORD>* outIndices)
{
	if (nullptr == outVertices || nullptr == outIndices)
	{
		return false;
	}

	outVertices->resize(4);
	(*outVertices)[0] = { {-1.0f, 1.0f},{0.0f, 0.0f} };
	(*outVertices)[1] = { {1.0f, 1.0f},{1.0f, 0.0f} };
	(*outVertices)[2] = { {1.0f, -1.0f},{1.0f, 1.0f} };
	(*outVertices)[3] = { {-1.0f, -1.0f},{0.0f, 1.0f} };

	outIndices->resize(6, 0);
	(*outIndices)[0] = { 0 };
	(*outIndices)[1] = { 1 };
	(*outIndices)[2] = { 2 };
	(*outIndices)[3] = { 0 };
	(*outIndices)[4] = { 2 };
	(*outIndices)[5] = { 3 };

	return true;
}
