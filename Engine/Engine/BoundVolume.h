#pragma once

struct BoundVolume_AABB
{
	Float3 minPos = { -1.0f, -1.0f ,-1.0f };
	Float3 maxPos = { 1.0f, 1.0f ,1.0f };;
};

class RenderComponent;

class BoundVolume
{
public:
	BoundVolume();

	~BoundVolume();

	bool Init(RenderComponent* pRenderComponent);

	void UpdateBoundVolume(IMesh* pMesh);

	bool IntersectionRayAABB(float* pOutT, const Float3& origin, const Float3& dir, float maxDistance);

private:
	bool CalculateBoundVolume
	(
		BoundVolume_AABB* pOutBoundVolume,
		E_VERTEX_FORMAT vertexFormat,
		uint32_t vertexStride,
		uint32_t vertexCount,
		void* pVertices
	);

	void CleanUp();

	IMesh* pMesh_;

	BoundVolume_AABB boundVolume_;
};
