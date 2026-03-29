#pragma once

constexpr int MAX_BONE_INFLUENCE = 4;

struct SkinWeight
{
	UINT   boneIndices[MAX_BONE_INFLUENCE] = { 0 };
	float  boneWeights[MAX_BONE_INFLUENCE] = { 0.0f };
};

struct SkeletalMeshInfo
{
	std::string meshName = "";
	int meshSubsetCount = 0;
	// Mesh 내부에 존재하는 Material "Element(레이어)" 개수.
	// 0 아님 1 -> 보통 1
	int materialElementCount = 1;
	int controlPointCount = 0;
	int polygonCount = 0;
	int deformerCount = 0;
	bool isTriangulated = false;
	bool isSkeletalMesh = false;

	std::vector<SkinnedMeshVertex> vertices;
	std::vector<std::vector<uint32_t>> indices;
	std::vector<Bone> bones;	
	std::vector<SkinWeight> skinDatas;

	SkeletalMeshInfo() = default;
	~SkeletalMeshInfo() = default;
	SkeletalMeshInfo(const SkeletalMeshInfo&) = default;
	SkeletalMeshInfo& operator=(const SkeletalMeshInfo&) = default;
	SkeletalMeshInfo(SkeletalMeshInfo&&) noexcept = default;
	SkeletalMeshInfo& operator=(SkeletalMeshInfo&&) noexcept = default;
};

struct SkeletalModel
{
	std::vector<SkeletalMeshInfo> meshInfos;

	struct BoneLink
	{
		std::string boneName;
		int32_t boneIndex;
		int32_t parentBoneIndex;
	};

	std::unordered_map<uint64_t, BoneLink> totalBoneMap;
};

struct StaticMeshInfo
{
	std::string meshName = "";
	int meshSubsetCount = 0;
	// Mesh 내부에 존재하는 Material "Element(레이어)" 개수.
	// 0 아님 1 -> 보통 1
	int materialElementCount = 1;
	int controlPointCount = 0;
	int polygonCount = 0;
	int deformerCount = 0;
	bool isTriangulated = false;
	bool isSkeletalMesh = false;
	
	std::vector<SimpleVertex> vertices;
	std::vector<std::vector<uint32_t>> indices;

	StaticMeshInfo() = default;
	~StaticMeshInfo() = default;
	StaticMeshInfo(const StaticMeshInfo&) = default;
	StaticMeshInfo& operator=(const StaticMeshInfo&) = default;
	StaticMeshInfo(StaticMeshInfo&&) noexcept = default;
	StaticMeshInfo& operator=(StaticMeshInfo&&) noexcept = default;
};

struct StaticModel
{
	std::vector<StaticMeshInfo> meshInfos;
};


struct TextureInfo
{
	std::string name;
	std::string filePath;
	std::string uvSet;

	// UV Transform
	float offsetU = 0.0f;
	float offsetV = 0.0f;
	float scaleU = 1.0f;
	float scaleV = 1.0f;

	bool wrapU = true;
	bool wrapV = true;
};

struct MaterialInfo
{
	// ---------- Basic ----------
	std::string name;
	std::string shadingModel;   // Lambert / Phong / PBR

	// ---------- Colors ----------
	float emissive[3] = { 0,0,0 };
	float ambient[3] = { 0,0,0 };
	float diffuse[3] = { 1,1,1 };
	float specular[3] = { 0,0,0 };
	float transparent[3] = { 0,0,0 };

	// ---------- Scalars ----------
	float opacity = 1.0f;
	float shininess = 0.0f;
	float specularFactor = 1.0f;
	float reflectionFactor = 0.0f;

	// ---------- Textures ----------
	TextureInfo diffuseTexture;
	TextureInfo normalTexture;
	TextureInfo specularTexture;
	TextureInfo emissiveTexture;
	TextureInfo opacityTexture;

	// ---------- Flags ----------
	bool hasDiffuseTexture = false;
	bool hasNormalTexture = false;
	bool hasSpecularTexture = false;
	bool hasEmissiveTexture = false;
	bool hasOpacityTexture = false;

	MaterialInfo() = default;
	~MaterialInfo() = default;
	MaterialInfo(const MaterialInfo&) = default;
	MaterialInfo& operator=(const MaterialInfo&) = default;
	MaterialInfo(MaterialInfo&&) noexcept = default;
	MaterialInfo& operator=(MaterialInfo&&) noexcept = default;
};