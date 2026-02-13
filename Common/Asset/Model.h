#pragma once

constexpr int MAX_BONE_INFLUENCE = 4;

struct Bone
{
	Float4x4 globalBindPose;
	Float4x4 localBindPose;
};

struct BoneInfo
{
	std::string name;
	int parentIndex;                // 부모 본 인덱스 (-1이면 루트)

	//Float4x4 globalBindPose;
	//Float4x4 localBindPose;
	//Float4x4   meshBindPose;       // Mesh Bind Global
	//Float4x4   boneBindPose;       // Bone Bind Global
};

struct SkinWeight
{
	UINT   boneIndices[MAX_BONE_INFLUENCE] = { 0 };
	float  boneWeights[MAX_BONE_INFLUENCE] = { 0.0f };
};

struct MeshInfo
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

	// Vertices
	std::vector<SkinnedMeshVertex> vertices;
	std::vector<std::vector<uint16_t>> indices;

	std::vector<Bone> bones;
	
	// Skins
	std::vector<SkinWeight> skinDatas;

	MeshInfo() = default;
	~MeshInfo() = default;
	MeshInfo(const MeshInfo&) = default;
	MeshInfo& operator=(const MeshInfo&) = default;
	MeshInfo(MeshInfo&&) noexcept = default;
	MeshInfo& operator=(MeshInfo&&) noexcept = default;
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

struct Model
{
	// Meshes
	std::vector<MeshInfo> meshInfos;

	// Bones
	std::vector<BoneInfo> totalBoneInfos;
	std::unordered_map<uint64_t, uint32_t> totalBoneMap;
};

