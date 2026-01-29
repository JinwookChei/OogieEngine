#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <Common/MathType/MathType.h>


struct SkinnedMeshVertex
{
	Float3 position;
	Float4 color;
	Float3 normal;
	Float2 uv;
	Float4 tangent;

	unsigned int boneIndices[4];
	float blendWeights[4];

	// unordered_map 전용.
	bool operator==(const SkinnedMeshVertex& other) const
	{
		return position.X == other.position.X
			&& position.Y == other.position.Y
			&& position.Z == other.position.Z
			&& color.X == other.color.X
			&& color.Y == other.color.Y
			&& color.Z == other.color.Z
			&& color.W == other.color.W
			&& normal.X == other.normal.X
			&& normal.Y == other.normal.Y
			&& normal.Z == other.normal.Z
			&& uv.X == other.uv.X
			&& uv.Y == other.uv.Y;
	}
};


inline void HashCombine(size_t& seed, size_t value)
{
	seed ^= value + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
}

inline size_t HashFloat(float f)
{
	uint32_t bits;
	memcpy(&bits, &f, sizeof(float));
	return std::hash<uint32_t>{}(bits);
}

template<>
struct std::hash<SkinnedMeshVertex>
{
	size_t operator()(const SkinnedMeshVertex& v) const noexcept
	{
		size_t seed = 0;

		HashCombine(seed, HashFloat(v.position.X));
		HashCombine(seed, HashFloat(v.position.Y));
		HashCombine(seed, HashFloat(v.position.Z));

		HashCombine(seed, HashFloat(v.color.X));
		HashCombine(seed, HashFloat(v.color.Y));
		HashCombine(seed, HashFloat(v.color.Z));
		HashCombine(seed, HashFloat(v.color.W));

		HashCombine(seed, HashFloat(v.normal.X));
		HashCombine(seed, HashFloat(v.normal.Y));
		HashCombine(seed, HashFloat(v.normal.Z));
		HashCombine(seed, HashFloat(v.normal.X));

		HashCombine(seed, HashFloat(v.uv.X));
		HashCombine(seed, HashFloat(v.uv.Y));

		return seed;
	}
};



//struct Bone
//{
//	std::string name_;
//	int parentIndex_;                // 부모 본 인덱스 (-1이면 루트)
//	Float4x4   meshBindPose_;       // Mesh Bind Global
//	Float4x4   boneBindPose_;       // Bone Bind Global
//};
//


struct Model
{
	std::vector<SkinnedMeshVertex> vertices_;
	std::vector<uint16_t> indices_;

	Model()
		: vertices_()
		, indices_()
	{
	}
	~Model()
	{
		CleanUp();
	}

	void CleanUp()
	{
		vertices_.clear();
		indices_.clear();
	}
};

