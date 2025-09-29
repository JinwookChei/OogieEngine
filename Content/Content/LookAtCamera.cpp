#include "stdafx.h"
#include "LookAtCamera.h"

LookAtCamera::LookAtCamera()
	: pRenderer_(new RenderComponent(this)),
	pTarget_(nullptr)
{
}

LookAtCamera::~LookAtCamera()
{
	CleanUp();
}

void LookAtCamera::Tick(double deltaTime)
{
	//Transform& targetTrans = pTarget_->GetWorldTransform();
	//Transform& thisTrans = this->GetWorldTransform();

	//Float4 mainPos = targetTrans.GetPosition();
	//Float4 subPos = thisTrans.GetPosition();

	//Float4 to;
	//VectorSub(to, mainPos, subPos);
	//Float3 to2 = { to.X,to.Y,to.Z };

	//Float4 rot;
	//VectorToEulerDeg(rot, to2);
	//
	//thisTrans.SetRotation(rot);
}

void LookAtCamera::BeginPlay()
{
	pRenderer_->Create(MESH_TYPE::CUBE);

	pTransform_->SetScale({ 2.0f, 2.0f, 2.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 20.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });

	
	/*pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 5.0f, 1.0f });*/
}

void LookAtCamera::Render()
{
	pRenderer_->Render();
}

void LookAtCamera::SetTarget(Actor* pTarget)
{
	if (nullptr == pTarget)
	{
		DEBUG_BREAK();
		return;
	}

	pTarget_ = pTarget;
}

void LookAtCamera::CleanUp()
{
	if (nullptr != pRenderer_)
	{
		delete pRenderer_;
		pRenderer_ = nullptr;
	}
}
