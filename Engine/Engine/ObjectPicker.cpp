#include "stdafx.h"
#include "Level.h"
#include "BoundVolume.h"
#include "ObjectPicker.h"


ObjectPicker::ObjectPicker()
	:pPickedActor_(nullptr),
	pickedMousePos_({ 0.0f, 0.0f })
{
}

ObjectPicker::~ObjectPicker()
{
	CleanUp();
}

ObjectPicker* ObjectPicker::GetInstance()
{
	return GObjectPicker;
}

void ObjectPicker::Tick(double deltaTime)
{
	if (InputManager::Instance()->IsDown(VK_LBUTTON))
	{
		pickedMousePos_ = InputManager::Instance()->GetCurrentMousePosition();
		RayCastFromScreen(pickedMousePos_);
	}
}

void ObjectPicker::RayCastFromScreen(const Float2& screenPos)
{
	Float4x4 invProjection;
	MATH::MatrixInverse(invProjection, GMainCamera->Projection());

	Float4x4 invView;
	MATH::MatrixInverse(invView, GMainCamera->View());

	float ndc_x = (2.0f * screenPos.X) / DEFAULT_SCREEN_WIDTH - 1.0f;
	float ndc_y = 1.0f - (2.0f * screenPos.Y) / DEFAULT_SCREEN_HEIGHT;
	float ndc_z = 0.0f;
	float ndc_w = 1.0f;
	Float4 ndcPos = { ndc_x, ndc_y, ndc_z, ndc_w };

	Float4 camSpacePos;
	MATH::MatrixMultiply(camSpacePos, ndcPos, invProjection );
	MATH::VectorScale(camSpacePos, camSpacePos, 1/ camSpacePos.W);

	Float4 worldSpacePoint;
	MATH::MatrixMultiply(worldSpacePoint, camSpacePos, invView );
	MATH::VectorScale(worldSpacePoint, worldSpacePoint, 1 / worldSpacePoint.W);

	Float4 camPos = GMainCamera->GetWorldTransform().GetPosition();
	Float3 st = { camPos.X, camPos.Y, camPos.Z };
	Float4 camDir = worldSpacePoint - camPos;
	Float4 camDirNorm;
	MATH::VectorNormalize(camDirNorm, camDir);
	Float3 dir = { camDirNorm.X, camDirNorm.Y, camDirNorm.Z };
	
	GDebugRenderer->DrawRay(st, dir, 100, { 1.0f, 0.0f, 0.0f, 1.0f });

	Level* pCurLevel = World::Instance()->GetLevel();
	LinkedList* pActorList = pCurLevel->GetActorList(E_ACTOR_TYPE::NORMAL);
	LINK_NODE* pActorIter = pActorList->GetHead();
	while (pActorIter)
	{
		Actor* pActor = static_cast<Actor*>(pActorIter->pItem_);

		Float4x4 invWorldMat;
		MATH::MatrixInverse(invWorldMat, pActor->GetWorldTransform().GetWorldMatrix());
		
		Float4 objSpaceCamPos;
		MATH::MatrixMultiply(objSpaceCamPos, camPos, invWorldMat);
		Float3 objSpaceCamPos3 = { objSpaceCamPos.X, objSpaceCamPos.Y, objSpaceCamPos.Z };

		Float4 objSpaceDir;
		MATH::MatrixMultiply(objSpaceDir, camDir, invWorldMat);
		Float3 objSpaceDir3 = { objSpaceDir.X, objSpaceDir.Y, objSpaceDir.Z };
		MATH::VectorNormalize(objSpaceDir3, objSpaceDir3);

		float distance; 
		if (true == pActor->GetBoundVolumeTEMP()->IntersectionRayAABB(&distance, objSpaceCamPos3, objSpaceDir3))
		{
			DEBUG_BREAK();
		}

		pActorIter = pActorIter->next_;

	}
}

Actor* ObjectPicker::GetPickedActor() const
{
	return GObjectPicker->pPickedActor_;
}

void ObjectPicker::CleanUp()
{

}




