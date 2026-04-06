#pragma once
#include "SceneAxisInfo.h"

class StaticModel;
class SkeletalModel;
class FBXImporter : public IFBXImporter
{
public:
	FBXImporter();
	~FBXImporter();
	
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
	ULONG __stdcall AddRef() override;
	ULONG __stdcall Release() override;

private:
	bool ImportStaticModel(StaticModel* pOutModel, const std::string& file) override;
	bool ImportSkeletalModel(SkeletalModel* pOutModel, const std::string& file) override;
	bool ImportAnimation(Animation* pOutAnimation, const std::string& file, double samplingRate = 1.0 / 60.0) override;

	bool Init(const std::string& file);
	fbxsdk::FbxMesh* FindMesh(fbxsdk::FbxNode* pNode);
	
	// Model
	void ExtractSkeletalMeshInfo(SkeletalModel* pModel, fbxsdk::FbxNode* pNode, int idx);
	void ExtractStaticMeshInfo(StaticModel* pModel, fbxsdk::FbxNode* pNode, int idx);
	bool ExtractNormal(Float3* pOutNormal, fbxsdk::FbxMesh* pMesh, int cpIndex, int polygonVertexIndex);
	bool ExtractTangent(Float4* pOutTangent, bool* pOutExistTangent, fbxsdk::FbxMesh* pMesh, int cpIndex, int polygonVertexIndex);
	bool ExtractUV(Float2* pOutUV, fbxsdk::FbxMesh* pMesh, int polyIndex, int vertexIndex);
	bool ExtractColor(Float4* pOutColor, fbxsdk::FbxMesh* pMesh, int cpIndex, int polygonVertexIndex);
	bool ExtractMaterialIndex(unsigned int* pOutIndex, fbxsdk::FbxMesh* pMesh, int polyIndex);
	void CalculateTangent(std::vector<SkinnedMeshVertex>* pVertices, const std::vector<std::vector<uint32_t>>& indices);
	void CalculateTangent(std::vector<StaticMeshVertex>* pVertices, const std::vector<std::vector<uint32_t>>& indices);
	
	// Skeleton
	void ExtractBones(SkeletalModel* pOutModel, FbxNode* pNode, int32_t parentBoneIndex);

	// Skin
	void ExtractBindBoneSkin(SkeletalMeshInfo* pOutMeshInfo, SkeletalModel& model, fbxsdk::FbxMesh* pMesh);
	void AddBoneWeight(SkinWeight& skinData, int boneIndex, float weight);
	void NormalizeSkinWeights(std::vector<SkinWeight>& skinData);
	void SkinDataToVertexData(SkeletalMeshInfo* pOutMeshInfo, const std::vector<uint32_t>& vertexCpIndexCache);

	// Material
	MaterialInfo ExtractMaterialInfo(fbxsdk::FbxSurfaceMaterial* material);
	void ReadColor(fbxsdk::FbxSurfaceMaterial* material, const char* propName, float outColor[3]);
	float GetScalar(fbxsdk::FbxSurfaceMaterial* material, const char* propName);
	bool ReadTexture(fbxsdk::FbxSurfaceMaterial* material, const char* propName, TextureInfo& outTex);


	// Animation
	void CollectSkeletonNodes(FbxNode* node, std::vector<FbxNode*>& outBones);


	int CountMeshes(fbxsdk::FbxNode* node);
	SceneAxisInfo GetSceneAxisInfo(fbxsdk::FbxScene* pScene);
	void CleanUp();

private:
	ULONG refCount_;

	fbxsdk::FbxManager* pManager_;
	fbxsdk::FbxIOSettings* pIOSetting_;
	fbxsdk::FbxImporter* pImporter_;
	fbxsdk::FbxScene* pScene_;

	fbxsdk::FbxAMatrix geoMatrix_;
	fbxsdk::FbxAMatrix globalMatrix_;

	SceneAxisInfo originSceneAxisInfo_;
	SceneAxisInfo convertSceneAxisInfo_;
};