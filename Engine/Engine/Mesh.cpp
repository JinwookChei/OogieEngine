#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh()
	:pVertex_(nullptr)
{
}

Mesh::~Mesh()
{
	if (nullptr != pVertex_)
	{
		pVertex_->Release();
		pVertex_ = nullptr;
	}
}

