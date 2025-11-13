#pragma once

class GeometryGenerator final
{
public:

	static bool CreateSphere(std::vector<SimpleVertex>* outVertices, std::vector<WORD>* outIndices, float radius = 0.5f);

	static bool CreateCube(std::vector<SimpleVertex>* outVertices, std::vector<WORD>* outIndices, float halfExtent = 0.5f);

	static bool CreateScreenRect(std::vector<ScreenRectVertex>* outVertices, std::vector<WORD>* outIndices);
};
