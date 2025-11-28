#include "stdafx.h"
#include "TransformWidget.h"
#include "Inspector.h"

Inspector::Inspector()
	:pPickedActor_(nullptr)
{
}

Inspector::~Inspector()
{
}

Inspector* Inspector::Create(IImGuiBindPickedActor* pPickedActor)
{
	return nullptr;
}

void Inspector::Render()
{
}

void Inspector::BindPickedActor(IImGuiBindPickedActor* pPickedActor)
{

}

void Inspector::CleanUp()
{

}
