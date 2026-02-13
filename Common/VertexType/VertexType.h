#pragma once

enum class E_VERTEX_FORMAT
{
	ScreenQuad = 0,
	Simple,
	SkinnedMesh
};

struct ScreenQuadVertex
{
	Float2 position;
	Float2 UV;
};

struct SimpleVertex
{
	Float3 position;
	Float4 color;
	Float3 normal;
	Float2 UV;
	Float4 tangent;

	SimpleVertex(Float3 pos, Float4 tmpcolor, Float3 tmpnormal, Float2 uv, Float4 tan)
		: position(pos), color(tmpcolor), normal(tmpnormal), UV(uv), tangent(tan) {
	}

	SimpleVertex(Float3 pos, Float4 tmpcolor, Float3 tmpnormal, Float2 uv)
		: position(pos), color(tmpcolor), normal(tmpnormal), UV(uv), tangent() {
	}
};

struct SkinnedMeshVertex
{
	Float3 position;
	Float4 color;
	Float3 normal;
	Float2 uv;
	Float4 tangent;

	unsigned int boneIndices[4];
	float blendWeights[4];
	unsigned int materialIndex;


	// unordered_map Àü¿ë.
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
			&& uv.Y == other.uv.Y
			&& materialIndex == other.materialIndex;
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


