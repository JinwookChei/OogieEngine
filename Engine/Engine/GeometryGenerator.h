#pragma once

class GeometryGenerator final
{
public:

	static bool CreateSphere(std::vector<StaticMeshVertex>* outVertices, std::vector<uint32_t>* outIndices, float radius = 0.5f);

	static bool CreateCube(std::vector<StaticMeshVertex>* outVertices, std::vector<uint32_t>* outIndices, float halfExtent = 0.5f);

	static bool CreateScreenQuad(std::vector<ScreenQuadVertex>* outVertices, std::vector<uint32_t>* outIndices);
};
