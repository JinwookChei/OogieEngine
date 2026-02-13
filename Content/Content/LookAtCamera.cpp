#include "stdafx.h"
#include "LookAtCamera.h"

LookAtCamera::LookAtCamera()
	:pFocusActor_(nullptr)
{
}

LookAtCamera::~LookAtCamera()
{
	CleanUp();
}

void LookAtCamera::Tick(double deltaTime)
{
	//const Transform& focusTrans = pFocusActor_->GetWorldTransform();

	//Vector eyePos = pTransform_->GetPosition();
	//Vector focusPos = focusTrans.GetPosition();

	//Float3 forwardVector = 
	//{
	//	focusPos.X - eyePos.X,
	//	focusPos.Y - eyePos.Y,
	//	focusPos.Z - eyePos.Z,
	//};

	//Float3 tmp;
	////VectorToEulerDeg(tmp, forwardVector);
	////ForwardToEulerDeg(tmp, forwardVector);

	//Float4 rot;
	//rot.X = tmp.X;
	//rot.Y = tmp.Y;
	//rot.Z = tmp.Z;
	//
	//pTransform_->SetRotation(rot);


	//const Transform& focusTrans = pFocusActor_->GetWorldTransform();

	//Float4 eyePos = pTransform_->GetPosition();
	//Float4 focusPos = focusTrans.GetPosition();

	//Float4 to = focusPos - eyePos;
	//Float4 worldUp = { 0.0f, 0.0f, 1.0f, 0.0f };
	//
	//Float4 right;
	//VectorCross(right, worldUp, to);
	//VectorNormalize(right, right);

	//Float4 up;
	//VectorCross(up, to, right);
	//VectorNormalize(up, up);

	//Float4x4 tmpMat; 
	//MatrixLookAtLH(tmpMat, eyePos, focusPos, up);
	////MatrixLookToLH(tmpMat, eyePos, to, up);

	//Float4 rotQ;
	//MatrixDecomposeFromRotQ(tmpMat, rotQ);

	//Float4 rotD;
	//QuaternionToEulerDeg(rotD, rotQ);

	//Float4 reRot;
	//reRot.X = rotD.X;
	//reRot.Y = rotD.Y;
	//reRot.Z = rotD.Z;

	//pTransform_->SetRotation(reRot);
	
}

void LookAtCamera::BeginPlay()
{
	IMesh* pMesh;
	IMaterial* pMaterial;
	MeshManager::Instance()->GetMesh(&pMesh, 1);
	MaterialManager::Instance()->GetMaterial(&pMaterial, 1);
	pRenderer_->InitPSO(1,1, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pRenderer_->GetPSO()->SetMeshToSlot(0, pMesh);
	pRenderer_->GetPSO()->SetMaterialToSlot(0, pMaterial);
	
	pTransform_->SetScale({ 2.0f, 2.0f, 2.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void LookAtCamera::Render()
{
	pRenderer_->Render();
}

void LookAtCamera::SetFocusActor(Actor* pActor)
{
	if (nullptr == pActor)
	{
		DEBUG_BREAK();
		return;
	}

	pFocusActor_ = pActor;
}

void LookAtCamera::CameraTransformUpdate()
{
	const Transform& worldTrans = GetWorldTransform();
	const Transform& focusTrans = pFocusActor_->GetWorldTransform();

	Vector eyePos = worldTrans.GetPosition();
	Vector eyeUp = worldTrans.UpVector();
	Vector focusPos = focusTrans.GetPosition();
	MATH::MatrixLookAtLH(view_, eyePos, focusPos, eyeUp);

	float fovRad = MATH::ConvertDegToRad(fov_);
	const Float2& size = GetRenderSize();
	MATH::MatrixPerspectiveFovLH(projection_, fov_, (size.X / size.Y), near_, far_);
}

void LookAtCamera::CleanUp()
{
}
