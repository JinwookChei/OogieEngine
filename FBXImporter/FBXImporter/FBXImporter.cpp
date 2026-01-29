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


	//fbxsdk::FbxAxisSystem engineAxis
	//(
	//	fbxsdk::FbxAxisSystem::eZAxis, fbxsdk::FbxAxisSystem::eParityEven, fbxsdk::FbxAxisSystem::eLeftHanded
	//);
	//engineAxis.ConvertScene(pScene_);


	sceneAxisInfo_ = GetSceneAxisInfo(pScene_);

	fbxsdk::FbxGeometryConverter geomConv(pManager_);
	geomConv.Triangulate(pScene_, true);

	return true;
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
	int totalMeshCount = CountMeshes(pRootNode);
	if (1 != totalMeshCount)
	{
		DEBUG_BREAK();
	}

	fbxsdk::FbxMesh* pMesh = FindMesh(pRootNode);
	if (nullptr == pMesh)
	{
		DEBUG_BREAK();
		return false;
	}


	int polygonCount = pMesh->GetPolygonCount();
	fbxsdk::FbxVector4* controlPoints = pMesh->GetControlPoints();

	pOutModel->vertices_.reserve(polygonCount * 3);
	pOutModel->indices_.reserve(polygonCount * 3);

	std::unordered_map<SkinnedMeshVertex, uint32_t> vertexCache;
	std::vector<uint32_t> vertexCpIndexCache;
	vertexCpIndexCache.reserve(polygonCount * 3);


	bool isExistTangent = false;
	int polygonVertexCounter = 0;
	for (int poly = 0; poly < polygonCount; ++poly)
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
				DEBUG_BREAK();
				return false;
			}
			bool res2 = ExtractUV(&v.uv, pMesh, cpIndex, polygonVertexCounter);
			if (false == res2)
			{
				DEBUG_BREAK();
				return false;
			}
			bool res3 = ExtractNormal(&v.normal, pMesh, cpIndex, polygonVertexCounter);
			if (false == res3)
			{
				DEBUG_BREAK();
				return false;
			}
			bool res4 = ExtractTangent(&v.tangent, &isExistTangent, pMesh, cpIndex, polygonVertexCounter);
			if (false == res4)
			{
				DEBUG_BREAK();
				return false;
			}
			
			polygonVertexCounter++;


			// 중복되는 Vertex들 최적화.
			uint32_t vertexIndex;
			auto iter = vertexCache.find(v);
			if (iter != vertexCache.end())
			{
				// cache에 이미 존재.
				vertexIndex = iter->second;
				pOutModel->indices_.push_back(vertexIndex);
			}
			else
			{
				// cache에 존재 하지 않음.
				vertexIndex = pOutModel->vertices_.size();
				pOutModel->vertices_.push_back(v);
				pOutModel->indices_.push_back(vertexIndex);

				vertexCpIndexCache.push_back(cpIndex);
				vertexCache[v] = vertexIndex;
			}
		}
	}

	if (false == isExistTangent)
	{
		CalculateTangent(&pOutModel->vertices_, pOutModel->indices_);
	}

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

bool FBXImporter::ExtractUV(Float2* pOutUV, fbxsdk::FbxMesh* pMesh, int polyIndex, int vertexIndex)
{
	int uvElementCnt = pMesh->GetElementUVCount();
	if (uvElementCnt != 1)
	{
		DEBUG_BREAK();
		return false;
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
	if (false == res)
	{
		DEBUG_BREAK();
		return false;
	}

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

void FBXImporter::CalculateTangent(std::vector<SkinnedMeshVertex>* pVertices, const std::vector<uint16_t>& indices)
{
	const size_t vertexCount = pVertices->size();

	std::vector<Float4> tanSum(vertexCount, { 0.0f,0.0f, 0.0f, 0.0f });
	std::vector<Float4> bitanSum(vertexCount, { 0.0f,0.0f, 0.0f, 0.0f });

	// 1. 삼각형 단위 누적
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		uint32_t i0 = indices[i + 0];
		uint32_t i1 = indices[i + 1];
		uint32_t i2 = indices[i + 2];

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
		UV0.Y = 0.0f;
		UV0.W = 0.0f;

		Float4 UV1;
		UV1.X = v1.uv.X;
		UV1.Y = v1.uv.Y;
		UV1.Y = 0.0f;
		UV1.W = 0.0f;

		Float4 UV2;
		UV2.X = v2.uv.X;
		UV2.Y = v2.uv.Y;
		UV2.Y = 0.0f;
		UV2.W = 0.0f;


		Float4 P0_to_P1;
		MATH::VectorSub(P0_to_P1, P1, P0);
		Float4 P0_to_P2;
		MATH::VectorSub(P0_to_P2, P2, P0);

		Float4 UV0_to_UV1;
		MATH::VectorSub(UV0_to_UV1, UV1, UV0);
		Float4 UV0_to_UV2;
		MATH::VectorSub(UV0_to_UV2, UV1, UV0);
		
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


int FBXImporter::CountMeshes(fbxsdk::FbxNode* node)
{
	int count = 0;

	// 현재 노드가 Mesh인지 검사
	if (node->GetMesh()) count++;

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
