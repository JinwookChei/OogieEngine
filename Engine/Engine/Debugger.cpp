#include "stdafx.h"
#include "Debugger.h"

Debugger* Debugger::GDebugger = nullptr;
Debugger::Debugger()
	: debugLines_()
	, pDebugLineMesh_(nullptr)
	, pDebugLineMaterial_(nullptr)
	, pDebugLinePSO_(nullptr)
{
	debugLines_.reserve(32768);

	MeshDesc debugLineMeshDesc;
	debugLineMeshDesc.primitiveType = E_MESH_PRIMITIVE_TYPE::LINE;
	debugLineMeshDesc.vertexType = E_VERTEX_TYPE::DEBUG_LINE;
	debugLineMeshDesc.usage = E_MESH_USAGE::DYNAMIC;
	debugLineMeshDesc.bindFlag = E_MESH_BIND_FLAG::VERTEX_BUFFER;
	debugLineMeshDesc.bufferSize = sizeof(DebugLineVertex) * 32768;
	debugLineMeshDesc.vertexFormatSize = sizeof(DebugLineVertex);
	debugLineMeshDesc.vertexCount = 0;
	debugLineMeshDesc.pVertices = nullptr;
	pDebugLineMesh_ = Renderer::GetFactory()->CreateMesh(debugLineMeshDesc);

	MaterialDesc debugLineMaterialDesc;
	debugLineMaterialDesc.shaderType = E_SHADER_PRESET::DEBUG_LINE;
	debugLineMaterialDesc.samplerState = E_SAMPLER_PRESET::DISABLE;
	debugLineMaterialDesc.blendState = E_BLEND_PRESET::OPAQUE_BLEND;
	debugLineMaterialDesc.textureNum = 0;
	pDebugLineMaterial_ = Renderer::GetFactory()->CreateMaterial(debugLineMaterialDesc);

	PipelineStateDesc psoDesc;
	psoDesc.meshSlotCount = 1;
	psoDesc.materialSlotCount = 1;
	psoDesc.depthState = E_DEPTH_PRESET::DEPTH_DISABLE;
	psoDesc.rasterizerMode = E_RASTERIZER_PRESET::DISABLE;
	pDebugLinePSO_ = Renderer::GetFactory()->CreatePipelineStateObject(psoDesc);
	pDebugLinePSO_->SetMeshToSlot(0, pDebugLineMesh_);
	pDebugLinePSO_->SetMaterialToSlot(0, pDebugLineMaterial_);
}

Debugger::~Debugger()
{
	CleanUp();
}

Debugger* Debugger::Create()
{
	Debugger::GDebugger = new Debugger;
	return GDebugger;
}

void Debugger::ShutDown()
{
	if (GDebugger != nullptr)
	{
		delete GDebugger;
		GDebugger = nullptr;
	}
}

void Debugger::Draw()
{
	if (GDebugger->debugLines_.empty())
	{
		return;
	}

	Renderer::Instance()->Render(GDebugger->pDebugLinePSO_);
}

void Debugger::DrawDebugLine(const Float3& start, const Float3& end, const Float4& color)
{
	GDebugger->debugLines_.push_back({start, color});
	GDebugger->debugLines_.push_back({end, color });
	GDebugger->pDebugLineMesh_->WriteBuffer(GDebugger->debugLines_.data(), sizeof(DebugLineVertex) * GDebugger->debugLines_.size());
}

void Debugger::DrawDebugRay(const Float3& origin, Float3& dir, float length, const Color& color)
{
	Float3 normDir;
	MATH::VectorNormalize(normDir, dir);
	Float3 d;
	MATH::VectorScale(d, normDir, length);
	Float3 endPos;
	MATH::VectorAdd(endPos, origin, d);
	DrawDebugLine(origin, endPos, color);
}


void Debugger::CleanUp()
{
	if (nullptr != pDebugLinePSO_)
	{
		pDebugLinePSO_->Release();
		pDebugLinePSO_ = nullptr;
	}
	if (nullptr != pDebugLineMesh_)
	{
		pDebugLineMesh_->Release();
		pDebugLineMesh_ = nullptr;
	}
	
	if (nullptr != pDebugLineMaterial_)
	{
		pDebugLineMaterial_->Release();
		pDebugLineMaterial_ = nullptr;
	}
}