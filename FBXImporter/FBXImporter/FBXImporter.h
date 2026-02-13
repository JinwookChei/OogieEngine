#pragma once
#include "SceneAxisInfo.h"


class Model;
class FBXImporter : public IFBXImporter
{
public:
	FBXImporter();
	~FBXImporter();
	
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
	ULONG __stdcall AddRef() override;
	ULONG __stdcall Release() override;

private:
	bool ImportModel(Model* pOutModel, const std::string& file) override;

	bool Init(const std::string& file);
	fbxsdk::FbxMesh* FindMesh(fbxsdk::FbxNode* pNode);
	
	// Model
	void ExtractMeshInfo(Model* pModel, fbxsdk::FbxNode* pNode, int idx);
	bool ExtractNormal(Float3* pOutNormal, fbxsdk::FbxMesh* pMesh, int cpIndex, int polygonVertexIndex);
	bool ExtractTangent(Float4* pOutTangent, bool* pOutExistTangent, fbxsdk::FbxMesh* pMesh, int cpIndex, int polygonVertexIndex);
	bool ExtractUV_1(Float2* pOutUV, fbxsdk::FbxMesh* pMesh, int polyIndex, int vertexIndex);
	bool ExtractUV_2(Float2* pOutUV, fbxsdk::FbxMesh* pMesh, int polyIndex, int vertexIndex);
	bool ExtractColor(Float4* pOutColor, fbxsdk::FbxMesh* pMesh, int cpIndex, int polygonVertexIndex);
	bool ExtractMaterialIndex(unsigned int* pOutIndex, fbxsdk::FbxMesh* pMesh, int polyIndex);
	void CalculateTangent(std::vector<SkinnedMeshVertex>* pVertices, const std::vector<std::vector<uint16_t>>& indices);
	
	// Skeleton
	//void ExtractBones(Model* pOutModel, FbxNode* pNode, uint32_t parentBoneIndex);
	void ExtractBones(Model* pOutModel, FbxNode* pNode, uint32_t parentBoneIndex);

	// Skin
	void ExtractBindBoneSkin(MeshInfo* outMeshInfo, Model& model, fbxsdk::FbxMesh* pMesh);
	void AddBoneWeight(SkinWeight& skinData, int boneIndex, float weight);
	void NormalizeSkinWeights(std::vector<SkinWeight>& skinData);


	// Material
	MaterialInfo ExtractMaterialInfo(fbxsdk::FbxSurfaceMaterial* material);
	//void ExtractMaterialInfo(MaterialInfo* pOutMaterialInfo, fbxsdk::FbxSurfaceMaterial* material);
	void ReadColor(fbxsdk::FbxSurfaceMaterial* material, const char* propName, float outColor[3]);
	float GetScalar(fbxsdk::FbxSurfaceMaterial* material, const char* propName);
	bool ReadTexture(fbxsdk::FbxSurfaceMaterial* material, const char* propName, TextureInfo& outTex);

	int CountMeshes(fbxsdk::FbxNode* node);
	SceneAxisInfo GetSceneAxisInfo(fbxsdk::FbxScene* pScene);
	void CleanUp();

private:
	ULONG refCount_;

	fbxsdk::FbxManager* pManager_;
	fbxsdk::FbxIOSettings* pIOSetting_;
	fbxsdk::FbxImporter* pImporter_;
	fbxsdk::FbxScene* pScene_;

	SceneAxisInfo originSceneAxisInfo_;
	SceneAxisInfo convertSceneAxisInfo_;
};