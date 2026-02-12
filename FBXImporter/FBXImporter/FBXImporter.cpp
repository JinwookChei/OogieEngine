#include "stdafx.h"
#include "SceneAxisInfo.h"
#include "FBXImporter.h"


FBXImporter::FBXImporter()
	: refCount_(1)
	, pManager_(nullptr)
	, pIOSetting_(nullptr)
	, pImporter_(nullptr)
	, pScene_(nullptr)
{
}

FBXImporter::~FBXImporter()
{
	CleanUp();
}

HRESULT __stdcall FBXImporter::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall FBXImporter::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall FBXImporter::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}


bool FBXImporter::ImportModel(Model* pOutModel, const std::string& file)
{
	if (!Init(file))
	{
		DEBUG_BREAK();
		return false;
	}

	if (nullptr == pOutModel)
	{
		DEBUG_BREAK();
		return false;
	}

	pOutModel->CleanUp();

	fbxsdk::FbxNode* pRootNode = pScene_->GetRootNode();

	ExtractMeshInfo(pOutModel, pRootNode, 0);
	if (1 != pOutModel->meshCount_)
	{
		// DEBUG_BREAK();
	}

	
	int k = 10;
	// TEMP
	//fbxsdk::FbxNode* pMeshNode = pMesh->GetNode();
	//int materialCount = pMeshNode->GetMaterialCount();
	//for (int i = 0; i < materialCount; ++i)
	//{
	//	fbxsdk::FbxSurfaceMaterial* pFbxMaterial = pMeshNode->GetMaterial(i);
	//	MaterialInfo materialInfo = BuildMaterialInfo(pFbxMaterial);

	//	//RegisterMaterial(materialInfo);
	//	int aa = 10;
	//}

	return true;
}


bool FBXImporter::Init(const std::string& file)
{
	CleanUp();

	pManager_ = fbxsdk::FbxManager::Create();
	if (nullptr == pManager_)
	{
		DEBUG_BREAK();
		return false;
	}

	pIOSetting_ = fbxsdk::FbxIOSettings::Create(pManager_, IOSROOT);
	pManager_->SetIOSettings(pIOSetting_);

	pImporter_ = fbxsdk::FbxImporter::Create(pManager_, "fbxImporter");
	bool status = pImporter_->Initialize(file.c_str(), -1, pManager_->GetIOSettings());
	if (false == status)
	{
		DEBUG_BREAK();
		return false;
	}

	pScene_ = fbxsdk::FbxScene::Create(pManager_, "fbxScene");
	pImporter_->Import(pScene_);


	originSceneAxisInfo_ = GetSceneAxisInfo(pScene_);
	const fbxsdk::FbxAxisSystem engineAxis(fbxsdk::FbxAxisSystem::eZAxis, fbxsdk::FbxAxisSystem::eParityEven, fbxsdk::FbxAxisSystem::eLeftHanded);
	const fbxsdk::FbxAxisSystem sceneAxis = pScene_->GetGlobalSettings().GetAxisSystem();
	if (sceneAxis != engineAxis)
	{
		engineAxis.ConvertScene(pScene_);
	}
	FbxSystemUnit targetUnit = FbxSystemUnit::m; // meters
	FbxSystemUnit sceneUnit = pScene_->GetGlobalSettings().GetSystemUnit();
	if (sceneUnit.GetScaleFactor() != targetUnit.GetScaleFactor())
	{
		targetUnit.ConvertScene(pScene_);
	}
	convertSceneAxisInfo_ = GetSceneAxisInfo(pScene_);


	fbxsdk::FbxGeometryConverter geomConv(pManager_);
	geomConv.Triangulate(pScene_, true);

	return true;
}


fbxsdk::FbxMesh* FBXImporter::FindMesh(fbxsdk::FbxNode* pNode)
{
	if (nullptr == pNode)
	{
		return nullptr;
	}

	fbxsdk::FbxMesh* mesh = pNode->GetMesh();
	if (nullptr != mesh)
	{
		return mesh;
	}

	for (int32_t n = 0; n < pNode->GetChildCount(); ++n)
	{
		fbxsdk::FbxMesh* findMesh = FindMesh(pNode->GetChild(n));
		if (nullptr != findMesh)
		{
			return findMesh;
		}
	}
	return nullptr;
}


void FBXImporter::ExtractMeshInfo(Model* pModel, fbxsdk::FbxNode* pNode, int idx)
{
	if (idx == 0)
	{
		pModel->meshCount_ = 0;
		pModel->meshInfo_.clear();
		pModel->meshInfo_.reserve(5);
	}

	if (nullptr == pNode)
	{
		return;
	}

	fbxsdk::FbxMesh* pMesh = pNode->GetMesh();
	if (nullptr != pMesh)
	{
		MeshInfo mInfo;
		mInfo.meshName = pMesh->GetName();
		mInfo.isTriangulated = pMesh->IsTriangleMesh();
		if (mInfo.isTriangulated == false)
		{
			DEBUG_BREAK();
			return;
		}
		mInfo.deformerCount = pMesh->GetDeformerCount();
		if (0 < mInfo.deformerCount)
		{
			mInfo.isSkeletalMesh = true;
		}
		else
		{
			mInfo.isSkeletalMesh = false;
		}
		mInfo.controlPointCount = pMesh->GetControlPointsCount();
		mInfo.materialElementCount = pMesh->GetElementMaterialCount();
		mInfo.materialSlotCount = pNode->GetMaterialCount();
		mInfo.polygonCount = pMesh->GetPolygonCount();

		mInfo.vertices_.reserve(mInfo.polygonCount*3);
		mInfo.indices_.resize(mInfo.materialSlotCount);
		for (int i = 0; i < mInfo.materialSlotCount; ++i)
		{
			mInfo.indices_[i].reserve(mInfo.polygonCount * 3);
		}

		fbxsdk::FbxVector4* controlPoints = pMesh->GetControlPoints();
		std::unordered_map<SkinnedMeshVertex, uint32_t> vertexCache;
		std::vector<uint32_t> vertexCpIndexCache;
		vertexCpIndexCache.reserve(mInfo.polygonCount * 3);

		bool isExistTangent = false;
		int polygonVertexCounter = 0;
		for (int poly = 0; poly < mInfo.polygonCount; ++poly)
		{
			int polySize = pMesh->GetPolygonSize(poly);
			if (polySize != 3)
			{
				DEBUG_BREAK();
			}

			for (int vert = 0; vert < polySize; ++vert)
			{
				int cpIndex = pMesh->GetPolygonVertex(poly, vert);

				// 여기서 모든 속성 추출
				SkinnedMeshVertex v;
				v.position.X = controlPoints[cpIndex][0];
				v.position.Y = controlPoints[cpIndex][1];
				v.position.Z = controlPoints[cpIndex][2];

				bool res1 = ExtractColor(&v.color, pMesh, cpIndex, polygonVertexCounter);
				if (false == res1)
				{
					//DEBUG_BREAK();
					//return false;
				}

				//bool res2 = ExtractUV_1(&v.uv, pMesh, cpIndex, polygonVertexCounter);
				bool res2 = ExtractUV_2(&v.uv, pMesh, poly, vert);
				if (false == res2)
				{
					DEBUG_BREAK();
					return;
				}
				bool res3 = ExtractNormal(&v.normal, pMesh, cpIndex, polygonVertexCounter);
				if (false == res3)
				{
					DEBUG_BREAK();
					return;
				}
				bool res4 = ExtractTangent(&v.tangent, &isExistTangent, pMesh, cpIndex, polygonVertexCounter);
				if (false == res4)
				{
					DEBUG_BREAK();
					return;
				}

				if (false == ExtractMaterialIndex(&v.materialIndex, pMesh, poly))
				{
					DEBUG_BREAK();
					return;
				}

				if (v.materialIndex >= mInfo.materialSlotCount)
				{
					DEBUG_BREAK();
					return;
				}

				polygonVertexCounter++;

				// 중복되는 Vertex들 최적화.
				uint32_t vertexIndex;
				auto iter = vertexCache.find(v);
				if (iter != vertexCache.end())
				{
					// cache에 이미 존재.
					vertexIndex = iter->second;
					unsigned int matIndex = v.materialIndex;
					if (matIndex >= 20)
					{
						DEBUG_BREAK();
						return;
					}
					mInfo.indices_[matIndex].push_back(vertexIndex);
				}
				else
				{
					// cache에 존재 하지 않음.
					vertexIndex = mInfo.vertices_.size();
					mInfo.vertices_.push_back(v);
					unsigned int matIndex = v.materialIndex;
					if (matIndex >= 20)
					{
						DEBUG_BREAK();
						return;
					}
					mInfo.indices_[matIndex].push_back(vertexIndex);
					vertexCpIndexCache.push_back(cpIndex);
					vertexCache[v] = vertexIndex;
				}
			}
		}

		if (false == isExistTangent)
		{
			CalculateTangent(&mInfo.vertices_, mInfo.indices_);
		}

		pModel->meshCount_++;
		pModel->meshInfo_.push_back(std::move(mInfo));
	}

	for (int32_t c = 0; c < pNode->GetChildCount(); ++c)
	{
		ExtractMeshInfo(pModel, pNode->GetChild(c), idx+1);
	}
}

bool FBXImporter::ExtractNormal(Float3* pOutNormal, fbxsdk::FbxMesh* pMesh, int cpIndex, int polygonVertexIndex)
{
	fbxsdk::FbxGeometryElementNormal* element = pMesh->GetElementNormal();
	if (!element)
	{
		DEBUG_BREAK();
		return false;
	}

	int index = 0;
	fbxsdk::FbxVector4 normal;
	switch (element->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		index = cpIndex;
		break;
	}
	case FbxGeometryElement::eByPolygonVertex:
	{
		index = polygonVertexIndex;
		break;
	}
	default:
		DEBUG_BREAK();
		return false;
	}

	switch (element->GetReferenceMode())
	{
	case FbxGeometryElement::eDirect:
	{
		normal = element->GetDirectArray().GetAt(index);
		break;
	}
	case FbxGeometryElement::eIndexToDirect:
	{
		int directIndex = element->GetIndexArray().GetAt(index);
		normal = element->GetDirectArray().GetAt(directIndex);
		break;
	}
	default:
		DEBUG_BREAK();
		return false;
	}

	pOutNormal->X = normal[0];
	pOutNormal->Y = normal[1];
	pOutNormal->Z = normal[2];
	return true;
}

bool FBXImporter::ExtractTangent(Float4* pOutTangent, bool* pOutExistTangent, fbxsdk::FbxMesh* pMesh, int cpIndex, int polygonVertexIndex)
{
	fbxsdk::FbxGeometryElementTangent* element = pMesh->GetElementTangent();
	if (nullptr == element)
	{
		*pOutExistTangent = false;
		return true;
	}
	else
	{
		*pOutExistTangent = true;
	}


	int index = 0;
	fbxsdk::FbxVector4 tangent;
	switch (element->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		index = cpIndex;
		break;
	}
	case FbxGeometryElement::eByPolygonVertex:
	{
		index = polygonVertexIndex;
		break;
	}
	default:
		DEBUG_BREAK();
		return false;
	}

	switch (element->GetReferenceMode())
	{
	case FbxGeometryElement::eDirect:
	{
		tangent = element->GetDirectArray().GetAt(index);
		break;
	}
	case FbxGeometryElement::eIndexToDirect:
	{
		int directIndex = element->GetIndexArray().GetAt(index);
		tangent = element->GetDirectArray().GetAt(directIndex);
		break;
	}
	default:
		DEBUG_BREAK();
		return false;
	}

	pOutTangent->X = tangent[0];
	pOutTangent->Y = tangent[1];
	pOutTangent->Z = tangent[2];
	pOutTangent->W = tangent[3];
	return true;
}

bool FBXImporter::ExtractUV_1(Float2* pOutUV, fbxsdk::FbxMesh* pMesh, int polyIndex, int vertexIndex)
{
	int uvElementCnt = pMesh->GetElementUVCount();
	if (uvElementCnt != 1)
	{
		//DEBUG_BREAK();
	}

	FbxGeometryElementUV* element = pMesh->GetElementUV(0);
	if (!element)
	{
		DEBUG_BREAK();
		return false;
	}

	int index = 0;
	switch (element->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		index = polyIndex;
		break;
	}
	case FbxGeometryElement::eByPolygonVertex:
	{
		index = vertexIndex;
		break;
	}
	default:
		return false;
	}

	switch (element->GetReferenceMode())
	{
	case FbxGeometryElement::eDirect:
	{
		FbxVector2 uv = element->GetDirectArray().GetAt(index);
		pOutUV->X = uv[0];
		pOutUV->Y = 1.0 - uv[1];
		return true;
	}
	case FbxGeometryElement::eIndexToDirect:
	{
		int directIndex = element->GetIndexArray().GetAt(index);
		FbxVector2 uv = element->GetDirectArray().GetAt(directIndex);
		pOutUV->X = uv[0];
		pOutUV->Y = 1.0 - uv[1];
		return true;
	}
	default:
		break;
	}

	return false;
}

bool FBXImporter::ExtractUV_2(Float2* pOutUV, fbxsdk::FbxMesh* pMesh, int polyIndex, int vertexIndex)
{
	int uvElementCnt = pMesh->GetElementUVCount();
	if (uvElementCnt != 1)
	{
		//DEBUG_BREAK();
		//return false;
	}

	fbxsdk::FbxGeometryElementUV* element = pMesh->GetElementUV(0);
	if (!element)
	{
		DEBUG_BREAK();
		return false;
	}

	const char* uvSetName = element->GetName();
	fbxsdk::FbxVector2 uv;
	bool unMapped;
	bool res = pMesh->GetPolygonVertexUV(polyIndex, vertexIndex, uvSetName, uv, unMapped);
	//if (false == res)
	//{
	//	// uvSetName에 매칭되는 uv값이 없는 경우.
	//	// uvElement는 여러개 일 수도 있음.
	//	uv[0] = 0.0;
	//	uv[1] = 0.0;
	//}

	pOutUV->X = uv[0];
	pOutUV->Y = 1.0f - uv[1];

	return true;
}

bool FBXImporter::ExtractColor(Float4* pOutColor, fbxsdk::FbxMesh* pMesh, int cpIndex, int polygonVertexIndex)
{
	if (pMesh->GetElementVertexColorCount() == 0)
	{
		return false;
	}

	fbxsdk::FbxGeometryElementVertexColor* element = pMesh->GetElementVertexColor(0);
	int index = 0;
	fbxsdk::FbxColor color;

	switch (element->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		index = cpIndex;
		break;
	}
	case FbxGeometryElement::eByPolygonVertex:
	{
		index = polygonVertexIndex;
		break;
	}
	default:
		DEBUG_BREAK();
		return false;
	}

	switch (element->GetReferenceMode())
	{
	case FbxGeometryElement::eDirect:
	{
		if (index >= element->GetDirectArray().GetCount())
		{
			DEBUG_BREAK();
			return false;
		}
		color = element->GetDirectArray().GetAt(index);
		break;
	}
	case FbxGeometryElement::eIndex:
	case FbxGeometryElement::eIndexToDirect:
	{
		if (index >= element->GetIndexArray().GetCount())
		{
			DEBUG_BREAK();
			return false;
		}

		int directIndex = element->GetIndexArray().GetAt(index);
		if (directIndex >= element->GetDirectArray().GetCount())
		{
			DEBUG_BREAK();
			return false;
		}

		color = element->GetDirectArray().GetAt(directIndex);
		break;
	}
	default:
		DEBUG_BREAK();
		return false;
	}

	pOutColor->X = color[0];
	pOutColor->Y = color[1];
	pOutColor->Z = color[2];
	pOutColor->W = color[3];

	return true;
}

bool FBXImporter::ExtractMaterialIndex(unsigned int* pOutIndex, fbxsdk::FbxMesh* pMesh, int polyIndex)
{
	FbxLayer* pLayer = pMesh->GetLayer(0);
	if (!pLayer)
	{
		DEBUG_BREAK();
		return false;
	}

	FbxLayerElementMaterial* pMatElem = pLayer->GetMaterials();
	if (!pMatElem)
	{
		return false;
	}

	fbxsdk::FbxLayerElement::EMappingMode mappingMode = pMatElem->GetMappingMode();
	fbxsdk::FbxLayerElement::EReferenceMode referenceMode = pMatElem->GetReferenceMode();

	if (mappingMode == FbxLayerElement::eAllSame)
	{
		//DEBUG_BREAK();
		*pOutIndex = 0;
		return true;
	}

	if (mappingMode == FbxLayerElement::eByPolygon)
	{
		if (referenceMode == FbxLayerElement::eIndexToDirect)
		{
			*pOutIndex = pMatElem->GetIndexArray().GetAt(polyIndex);
			if (*pOutIndex == 0)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 1)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 2)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 3)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 4)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 5)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 6)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 7)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 8)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 9)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 10)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 11)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 12)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 13)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 14)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 15)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 16)
			{
				int qqq = 10;
			}
			if (*pOutIndex == 17)
			{
				int qqq = 10;
			}
			return true;
		}
		else if (referenceMode == FbxLayerElement::eDirect)
		{
			*pOutIndex = polyIndex;
			if (*pOutIndex != 0)
			{
				DEBUG_BREAK();
			}
			return true;
		}
	}

	DEBUG_BREAK();
	return false;
}

void FBXImporter::CalculateTangent(std::vector<SkinnedMeshVertex>* pVertices, const std::vector<std::vector<uint16_t>>& indices)
{
	const size_t vertexCount = pVertices->size();

	std::vector<Float4> tanSum(vertexCount, { 0.0f,0.0f, 0.0f, 0.0f });
	std::vector<Float4> bitanSum(vertexCount, { 0.0f,0.0f, 0.0f, 0.0f });

	// 1. 삼각형 단위 누적

	for (size_t j = 0; j < indices.size(); ++j)
	{
		for (size_t i = 0; i < indices[j].size(); i += 3)
		{
			uint32_t i0 = indices[j][i + 0];
			uint32_t i1 = indices[j][i + 1];
			uint32_t i2 = indices[j][i + 2];

			const SkinnedMeshVertex& v0 = (*pVertices)[i0];
			const SkinnedMeshVertex& v1 = (*pVertices)[i1];
			const SkinnedMeshVertex& v2 = (*pVertices)[i2];

			Float4 P0;
			P0.X = v0.position.X;
			P0.Y = v0.position.Y;
			P0.Z = v0.position.Z;
			P0.W = 0.0f;

			Float4 P1;
			P1.X = v1.position.X;
			P1.Y = v1.position.Y;
			P1.Z = v1.position.Z;
			P1.W = 0.0f;

			Float4 P2;
			P2.X = v2.position.X;
			P2.Y = v2.position.Y;
			P2.Z = v2.position.Z;
			P2.W = 0.0f;

			Float4 UV0;
			UV0.X = v0.uv.X;
			UV0.Y = v0.uv.Y;
			UV0.Z = 0.0f;
			UV0.W = 0.0f;

			Float4 UV1;
			UV1.X = v1.uv.X;
			UV1.Y = v1.uv.Y;
			UV1.Z = 0.0f;
			UV1.W = 0.0f;

			Float4 UV2;
			UV2.X = v2.uv.X;
			UV2.Y = v2.uv.Y;
			UV2.Z = 0.0f;
			UV2.W = 0.0f;


			Float4 P0_to_P1;
			MATH::VectorSub(P0_to_P1, P1, P0);
			Float4 P0_to_P2;
			MATH::VectorSub(P0_to_P2, P2, P0);

			Float4 UV0_to_UV1;
			MATH::VectorSub(UV0_to_UV1, UV1, UV0);
			Float4 UV0_to_UV2;
			MATH::VectorSub(UV0_to_UV2, UV2, UV0);

			float denom = UV0_to_UV1.X * UV0_to_UV2.Y - UV0_to_UV2.X * UV0_to_UV1.Y;
			if (fabs(denom) < 1e-6f) continue;
			float f = 1.0f / denom;


			Float4 E1;
			MATH::VectorScale(E1, P0_to_P1, UV0_to_UV2.Y);
			Float4 E2;
			MATH::VectorScale(E2, P0_to_P2, UV0_to_UV1.Y);
			Float4 SubE1;
			MATH::VectorSub(SubE1, E1, E2);
			Float4 T;
			MATH::VectorScale(T, SubE1, f);


			Float4 E3;
			MATH::VectorScale(E3, P0_to_P2, UV0_to_UV1.X);
			Float4 E4;
			MATH::VectorScale(E4, P0_to_P1, UV0_to_UV2.X);
			Float4 SubE2;
			MATH::VectorSub(SubE2, E3, E4);
			Float4 B;
			MATH::VectorScale(B, SubE2, f);

			MATH::VectorAdd(tanSum[i0], tanSum[i0], T);
			MATH::VectorAdd(tanSum[i1], tanSum[i1], T);
			MATH::VectorAdd(tanSum[i2], tanSum[i2], T);

			MATH::VectorAdd(bitanSum[i0], bitanSum[i0], B);
			MATH::VectorAdd(bitanSum[i1], bitanSum[i1], B);
			MATH::VectorAdd(bitanSum[i2], bitanSum[i2], B);
		}
	}
	// 2. Vertex 단위 정규화 + Gram-Schmidt
	for (size_t i = 0; i < vertexCount; ++i)
	{
		Float4 N;
		N.X = (*pVertices)[i].normal.X;
		N.Y = (*pVertices)[i].normal.Y;
		N.Z = (*pVertices)[i].normal.Z;
		N.W = 0.0f;
		Float4 T = tanSum[i];
		Float4 B = bitanSum[i];


		// T 외적 N
		// 정사영 제거(Gram-Schmidt 정규직교화)
		//T = DirectX::XMVector3Normalize(T - N * DirectX::XMVector3Dot(N, T));
		float d;
		MATH::VectorDot(d, N, T);
		Float4 sN;
		MATH::VectorScale(sN, N, d);
		Float4 subN;
		MATH::VectorSub(subN, T, sN);
		MATH::VectorNormalize(T, subN);

		// handedness
		//float handedness = (DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVector3Cross(N, T), B)) < 0.0f) ? -1.0f : 1.0f;
		Float3 N3;
		N3.X = N.X;
		N3.Y = N.Y;
		N3.Z = N.Z;

		Float3 T3;
		T3.X = T.X;
		T3.Y = T.Y;
		T3.Z = T.Z;

		Float3 B3;
		B3.X = B.X;
		B3.Y = B.Y;
		B3.Z = B.Z;

		Float3 Cross;
		MATH::VectorCross(Cross, N3, T3);
		float Dot;
		MATH::VectorDot(Dot, Cross, B3);
		float handedness = Dot < 0.0f ? -1.0f : 1.0f;

		(*pVertices)[i].tangent = T;
		(*pVertices)[i].tangent.W = handedness;
	}
}

MaterialInfo FBXImporter::BuildMaterialInfo(fbxsdk::FbxSurfaceMaterial* material)
{
	MaterialInfo info;
	if (!material)
		return info;

	info.Name = material->GetName();
	info.ShadingModel = material->ShadingModel.Get().Buffer();

	// Color Properties
	ReadColor(material, fbxsdk::FbxSurfaceMaterial::sEmissive, info.Emissive);
	ReadColor(material, fbxsdk::FbxSurfaceMaterial::sAmbient, info.Ambient);
	ReadColor(material, fbxsdk::FbxSurfaceMaterial::sDiffuse, info.Diffuse);
	ReadColor(material, fbxsdk::FbxSurfaceMaterial::sSpecular, info.Specular);
	ReadColor(material, fbxsdk::FbxSurfaceMaterial::sTransparentColor, info.Transparent);

	// Scalar
	info.Opacity = 1.0f - GetScalar(material, fbxsdk::FbxSurfaceMaterial::sTransparencyFactor);
	info.Shininess = GetScalar(material, fbxsdk::FbxSurfaceMaterial::sShininess);
	info.ReflectionFactor = GetScalar(material, fbxsdk::FbxSurfaceMaterial::sReflectionFactor);

	// Phong Only
	if (material->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		info.SpecularFactor = GetScalar(material, fbxsdk::FbxSurfacePhong::sSpecularFactor);
	}

	// Textures
	info.HasDiffuseTexture = ReadTexture(material, fbxsdk::FbxSurfaceMaterial::sDiffuse, info.DiffuseTexture);
	info.HasNormalTexture = ReadTexture(material, fbxsdk::FbxSurfaceMaterial::sNormalMap, info.NormalTexture);
	info.HasSpecularTexture = ReadTexture(material, fbxsdk::FbxSurfaceMaterial::sSpecular, info.SpecularTexture);
	info.HasEmissiveTexture = ReadTexture(material, fbxsdk::FbxSurfaceMaterial::sEmissive, info.EmissiveTexture);
	info.HasOpacityTexture = ReadTexture(material, fbxsdk::FbxSurfaceMaterial::sTransparentColor, info.OpacityTexture);

	return info;
}

void FBXImporter::ReadColor(fbxsdk::FbxSurfaceMaterial* material, const char* propName, float outColor[3])
{
	fbxsdk::FbxProperty prop = material->FindProperty(propName);
	if (!prop.IsValid())
	{
		return;
	}

	fbxsdk::FbxDouble3 v = prop.Get<FbxDouble3>();
	outColor[0] = (float)v[0];
	outColor[1] = (float)v[1];
	outColor[2] = (float)v[2];
}

float FBXImporter::GetScalar(fbxsdk::FbxSurfaceMaterial* material, const char* propName)
{
	fbxsdk::FbxProperty prop = material->FindProperty(propName);
	if (!prop.IsValid())
	{
		return 0.0f;
	}

	return (float)prop.Get<fbxsdk::FbxDouble>();
}

bool FBXImporter::ReadTexture(fbxsdk::FbxSurfaceMaterial* material, const char* propName, TextureInfo& outTex)
{
	fbxsdk::FbxProperty prop = material->FindProperty(propName);
	if (!prop.IsValid())
	{
		return false;
	}
	if (prop.GetSrcObjectCount<fbxsdk::FbxTexture>() == 0)
	{
		return false;
	}

	fbxsdk::FbxTexture* tex = prop.GetSrcObject<fbxsdk::FbxTexture>(0);
	if (!tex || !tex->GetClassId().Is(fbxsdk::FbxFileTexture::ClassId))
	{
		return false;
	}

	fbxsdk::FbxFileTexture* fileTex = (fbxsdk::FbxFileTexture*)tex;
	outTex.Name = fileTex->GetName();
	outTex.FilePath = fileTex->GetFileName();
	outTex.UVSet = fileTex->UVSet.Get().Buffer();
	outTex.OffsetU = (float)fileTex->GetTranslationU();
	outTex.OffsetV = (float)fileTex->GetTranslationV();
	outTex.ScaleU = (float)fileTex->GetScaleU();
	outTex.ScaleV = (float)fileTex->GetScaleV();
	outTex.WrapU = fileTex->GetWrapModeU() != fbxsdk::FbxTexture::eClamp;
	outTex.WrapV = fileTex->GetWrapModeV() != fbxsdk::FbxTexture::eClamp;

	return true;
}


int FBXImporter::CountMeshes(fbxsdk::FbxNode* node)
{
	int count = 0;

	// 현재 노드가 Mesh인지 검사
	fbxsdk::FbxMesh* pMesh = node->GetMesh();
	if (pMesh)
	{
		count++;
	}

	// 자식 노드 순회
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		count += CountMeshes(node->GetChild(i));
	}

	return count;
}


SceneAxisInfo FBXImporter::GetSceneAxisInfo(fbxsdk::FbxScene* pScene)
{
	SceneAxisInfo axisInfo{};

	if (!pScene) return axisInfo;

	fbxsdk::FbxGlobalSettings& globalSettings = pScene->GetGlobalSettings();
	fbxsdk::FbxAxisSystem axisSystem = globalSettings.GetAxisSystem();

	// Up Vector
	axisInfo.UpAxis = axisSystem.GetUpVector(axisInfo.UpSign);

	// Front Vector
	axisInfo.FrontAxis = axisSystem.GetFrontVector(axisInfo.FrontSign);

	// Coordinate System (Left / Right Handed)
	axisInfo.CoordSystem = axisSystem.GetCoorSystem();

	// Unit (cm 기준)
	fbxsdk::FbxSystemUnit systemUnit = globalSettings.GetSystemUnit();
	axisInfo.UnitScale = systemUnit.GetScaleFactor();

	return axisInfo;
}


void FBXImporter::CleanUp()
{
	if (pScene_)
	{
		pScene_->Destroy();
		pScene_ = nullptr;
	}
	if (pImporter_)
	{
		pImporter_->Destroy();
		pImporter_ = nullptr;
	}
	if (pIOSetting_)
	{
		pIOSetting_->Destroy();
		pIOSetting_ = nullptr;
	}
	if (pManager_)
	{
		pManager_->Destroy();
		pManager_ = nullptr;
	}
}
