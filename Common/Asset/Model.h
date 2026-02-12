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
	unsigned int materialIndex;


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



//struct Bone
//{
//	std::string name_;
//	int parentIndex_;                // 부모 본 인덱스 (-1이면 루트)
//	Float4x4   meshBindPose_;       // Mesh Bind Global
//	Float4x4   boneBindPose_;       // Bone Bind Global
//};
//

struct MeshInfo
{
	std::string meshName = "";
	std::vector<SkinnedMeshVertex> vertices_;
	std::vector<std::vector<uint16_t>> indices_;

	// Mesh 내부에 존재하는 Material "Element(레이어)" 개수.
	// 0 아님 1 -> 보통 1
	int materialElementCount = 1;

	// Node에 실제로 바인딩된 Material 슬롯 개수
	int materialSlotCount = 0;

	int controlPointCount =0;
	int polygonCount = 0;
	int deformerCount = 0;

	bool isTriangulated = false;
	bool isSkeletalMesh = false;


	MeshInfo() = default;
	~MeshInfo() = default;
	MeshInfo(const MeshInfo&) = default;
	MeshInfo& operator=(const MeshInfo&) = default;
	MeshInfo(MeshInfo&&) noexcept = default;
	MeshInfo& operator=(MeshInfo&&) noexcept = default;
};

struct TextureInfo
{
	std::string Name;
	std::string FilePath;
	std::string UVSet;

	// UV Transform
	float OffsetU = 0.0f;
	float OffsetV = 0.0f;
	float ScaleU = 1.0f;
	float ScaleV = 1.0f;

	bool WrapU = true;
	bool WrapV = true;
};

struct MaterialInfo
{
	// ---------- Basic ----------
	std::string Name;
	std::string ShadingModel;   // Lambert / Phong / PBR

	// ---------- Colors ----------
	float Emissive[3] = { 0,0,0 };
	float Ambient[3] = { 0,0,0 };
	float Diffuse[3] = { 1,1,1 };
	float Specular[3] = { 0,0,0 };
	float Transparent[3] = { 0,0,0 };

	// ---------- Scalars ----------
	float Opacity = 1.0f;
	float Shininess = 0.0f;
	float SpecularFactor = 1.0f;
	float ReflectionFactor = 0.0f;

	// ---------- Textures ----------
	TextureInfo DiffuseTexture;
	TextureInfo NormalTexture;
	TextureInfo SpecularTexture;
	TextureInfo EmissiveTexture;
	TextureInfo OpacityTexture;

	// ---------- Flags ----------
	bool HasDiffuseTexture = false;
	bool HasNormalTexture = false;
	bool HasSpecularTexture = false;
	bool HasEmissiveTexture = false;
	bool HasOpacityTexture = false;
};

struct Model
{
	// 아직은 mesh 1개만 대응 함. (0번 index Mesh로만 활용.)
	std::vector<MeshInfo> meshInfo_;
	uint8_t meshCount_;
	Model() 
	: meshInfo_()
	, meshCount_(0)
	{ 
	}
	~Model()
	{
		CleanUp();
	}

	void CleanUp()
	{
		meshInfo_.clear();
	}
};

