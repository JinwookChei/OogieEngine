#include "stdafx.h"
#include "Mesh.h"


Mesh::Mesh(IVertex* pVertex)
	: pVertexImpl_(pVertex)
{
}

Mesh::~Mesh()
{
	CleanUp();
}

void Mesh::Setting()
{
	if (nullptr == pVertexImpl_)
	{
		DEBUG_BREAK();
		return;
	}

	pVertexImpl_->Setting();
}

bool Mesh::AddInputLayout(const char* semanticName, uint32_t semanticIndex, uint32_t format, uint32_t inputSlot, bool isInstanceData)
{
	if (nullptr == pVertexImpl_)
	{
		DEBUG_BREAK();
		return false;
	}

	return pVertexImpl_->AddInputLayout(semanticName, semanticIndex, format, inputSlot, isInstanceData);
}

bool Mesh::Draw()
{
	if (nullptr == pVertexImpl_)
	{
		DEBUG_BREAK();
		return false;
	}

	return pVertexImpl_->Draw();
}

void Mesh::SetVertex(IVertex* pVertex)
{
	if (nullptr != pVertexImpl_)
	{
		pVertexImpl_->Release();
		pVertexImpl_ = nullptr;
	}

	if (nullptr != pVertex)
	{
		pVertexImpl_ = pVertex;
		pVertexImpl_->AddRef();
	}
}

IVertex* Mesh::GetVertex() const
{
	return pVertexImpl_;
}


void Mesh::CleanUp()
{
	if (nullptr != pVertexImpl_)
	{
		pVertexImpl_->Release();
		pVertexImpl_ = nullptr;
	}
}

