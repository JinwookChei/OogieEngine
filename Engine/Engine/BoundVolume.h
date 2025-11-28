#pragma once

class RenderComponent;
class Transform;

class BoundVolume
{
public:
	BoundVolume();

	~BoundVolume();

	bool Init(RenderComponent* pRenderComponent, Transform* pTransform);

	void UpdateBoundVolume(IMesh* pMesh);

	const AABB& GetAABB() const;

private:
	bool CalculateAABB
	(
		AABB* pOutAABB,
		E_VERTEX_FORMAT vertexFormat,
		uint32_t vertexStride,
		uint32_t vertexCount,
		void* pVertices
	);

	void CleanUp();

	IMesh* pMesh_;

	AABB aabb_;
};
