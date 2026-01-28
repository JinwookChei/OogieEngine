#pragma once
class Model;

class AssetImporter : public IAssetImporter
{
public:
	AssetImporter();
	~AssetImporter();
	
private:
	bool Init(const std::string& file);

	bool ImportModel(Model* pOutModel, const std::string& file);
	fbxsdk::FbxMesh* FindMesh(fbxsdk::FbxNode* pNode);
	
	bool ExtractNormal(Float3* pOutNormal, fbxsdk::FbxMesh* pMesh, int cpIndex, int polygonVertexIndex);
	bool ExtractTangent(Float4* pOutTangent, bool* pOutExistTangent, fbxsdk::FbxMesh* pMesh, int cpIndex, int polygonVertexIndex);
	bool ExtractUV(Float2* pOutUV, fbxsdk::FbxMesh* pMesh, int polyIndex, int vertexIndex);
	bool ExtractColor(Float4* pOutColor, fbxsdk::FbxMesh* pMesh, int cpIndex, int polygonVertexIndex);
	void CalculateTangent(std::vector<SkinnedMeshVertex>* pVertices, const std::vector<uint16_t>& indices);
	


	int CountMeshes(fbxsdk::FbxNode* node);
	SceneAxisInfo GetSceneAxisInfo(fbxsdk::FbxScene* pScene);
	void CleanUp();

private:
	fbxsdk::FbxManager* pManager_;
	fbxsdk::FbxIOSettings* pIOSetting_;
	fbxsdk::FbxImporter* pImporter_;
	fbxsdk::FbxScene* pScene_;

	SceneAxisInfo sceneAxisInfo_;
};