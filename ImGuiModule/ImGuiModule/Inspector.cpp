#include "stdafx.h"
#include "TransformWidget.h"
#include "Inspector.h"

Inspector::Inspector()
	:pPickedActor_(nullptr),
	pTransformWidget_(nullptr)
{
}

Inspector::~Inspector()
{
	CleanUp();
}

Inspector* Inspector::Create()
{
	Inspector* pNewInspector = new Inspector;
	pNewInspector->Init();

	return pNewInspector;
}

bool Inspector::Init()
{
	pTransformWidget_ = new TransformWidget;
	
	return true;
}


void Inspector::Render()
{
	if (!IsActive())
	{
		return;
	}

	pTransformWidget_->Render();
}

void Inspector::BindPickedActor(IImGuiBindPickedActor* pPickedActor)
{
	pPickedActor_ = pPickedActor;
	pTransformWidget_->BindPickedActor(pPickedActor_);
}

void Inspector::CleanUp()
{
	if (nullptr != pTransformWidget_)
	{
		pTransformWidget_->Release();
		pTransformWidget_ = nullptr;
	}

	if (nullptr != pPickedActor_)
	{
		pPickedActor_ = nullptr;
	}
}
