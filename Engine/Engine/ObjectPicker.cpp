#include "stdafx.h"
#include "ObjectPicker.h"

ObjectPicker::ObjectPicker()
	:pPickedActor_(nullptr),
	pickedMousePos_({0.0f, 0.0f})
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
		Float4 camPos = GMainCamera->GetWorldTransform().GetPosition();
		Float3 st = { camPos.X, camPos.Y, camPos.Z };
		Float4 camDir = GMainCamera->GetWorldTransform().ForwardVector();
		Float3 dir = { camDir.X, camDir.Y, camDir.Z };
		GDebugRenderer->DrawRay(st, dir, 100.0f, {1.0f, 0.0f, 0.0f, 1.0f});

		pickedMousePos_ = InputManager::Instance()->GetCurrentMousePosition();
		RayCastFromScreen(pickedMousePos_);
	}
}

void ObjectPicker::RayCastFromScreen(const Float2& screenPos)
{
	// Calc WorldPos From Depth + PixelPosition
	//float screen_x = input.uv.x * ScreenResolution.x;
	//float screen_y = input.uv.y * ScreenResolution.y;
	
	Float4x4 invProjection;
	MATH::MatrixInverse(invProjection, GMainCamera->Projection());

	Float4x4 invView;
	MATH::MatrixInverse(invView, GMainCamera->View());

	
	float ndc_x = (2.0f * screenPos.X) / DEFAULT_SCREEN_WIDTH - 1.0f;
	float ndc_y = 1.0f - (2.0f * screenPos.Y) / DEFAULT_SCREEN_HEIGHT;
	float ndc_z = 0.0f;
	float ndc_w = 1.0f;
	Float4 ndcPos = { ndc_x, ndc_y, ndc_z, ndc_w };

	Float4 camSpacePoint;
	MATH::MatrixMultiply(camSpacePoint, invProjection, ndcPos);

	Float4 worldSpacePoint;
	MATH::MatrixMultiply(worldSpacePoint, invView, camSpacePoint);

	//float4 viewPos = mul(ndcPos, InvProjectTransform);
	//viewPos /= viewPos.w;
	//float4 worldPos = mul(viewPos, InvViewTransform);
}

Actor* ObjectPicker::GetPickedActor() const
{
	return GObjectPicker->pPickedActor_;
}

void ObjectPicker::CleanUp()
{

}




