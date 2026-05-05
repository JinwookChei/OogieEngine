#include "stdafx.h"
#include "RunTimeMode.h"
#include "EditorCamera.h"

EditorCamera::EditorCamera()
{
	cameraSpeed_ = 8.0f;
}

EditorCamera::~EditorCamera()
{
	CleanUp();
}

void EditorCamera::Tick(double deltaTime)
{
	Camera::Tick(deltaTime);

	if (RunTimeMode::GetCurrentMode() != E_RUNTIME_MODE::EDITOR) return;
	if (InputManager::IsPress(VK_RBUTTON))
	{
		const Float2& deltaMouseMove = InputManager::GetDeltaMouseMove();
		pTransform_->AddRotaionZ(deltaMouseMove.X * deltaTime * 30.0f);
		pTransform_->AddRotaionY(deltaMouseMove.Y * deltaTime * 30.0f);
	}

	if (InputManager::IsPress('W'))
	{
		Float4 forwardVector = pTransform_->ForwardVector();
		Float4 offset;
		MATH::VectorScale(offset, forwardVector, cameraSpeed_ * (float)deltaTime);
		pTransform_->AddPosition(offset);
	}
	if (InputManager::IsPress('S'))
	{
		Float4 forwardVector = pTransform_->ForwardVector();
		Float4 offset;
		MATH::VectorScale(offset, forwardVector, -1.0f * cameraSpeed_ * (float)deltaTime);
		pTransform_->AddPosition(offset);
	}
	if (InputManager::IsPress('A'))
	{
		Float4 rightVector = pTransform_->RightVector();
		Float4 offset;
		MATH::VectorScale(offset, rightVector, -1.0f * cameraSpeed_ * (float)deltaTime);
		pTransform_->AddPosition(offset);
	}
	if (InputManager::IsPress('D'))
	{
		Float4 rightVector = pTransform_->RightVector();
		Float4 offset;
		MATH::VectorScale(offset, rightVector, cameraSpeed_ * (float)deltaTime);
		pTransform_->AddPosition(offset);
	}
	if (InputManager::IsPress('F'))
	{
		pTransform_->AddRotaionY(1.0f);
	}
	if (InputManager::IsPress('R'))
	{
		pTransform_->AddRotaionY(-1.0f);
	}
	if (InputManager::IsPress('Q'))
	{
		pTransform_->AddRotaionZ(-50 * deltaTime);
	}
	if (InputManager::IsPress('E'))
	{
		pTransform_->AddRotaionZ(50 * deltaTime);
	}

	if (InputManager::IsPress('K'))
	{
		
	// 1. 시간 기반 회전각 업데이트
		yaw += rotationSpeed * deltaTime;

		// 2. 라디안 변환
		float radYaw = MATH::ConvertDegToRad(yaw);
		float radPitch = MATH::ConvertDegToRad(pitch);

		// 3. 왼손 좌표계 (X-Front, Y-Right, Z-Up) 맞춤형 위치 계산
		// cos(Pitch)는 바닥 평면으로 투영된 길이를 의미합니다.
		float horizontalDistance = radius * cosf(radPitch);

		// X가 앞(Front), Y가 오른쪽(Right)이므로
		// Yaw가 0일 때 (cos=1, sin=0) 정면인 X축에 위치하게 됩니다.
		float posX = targetPos.X + horizontalDistance * cosf(radYaw);
		float posY = targetPos.Y + horizontalDistance * sinf(radYaw);
		float posZ = targetPos.Z + radius * sinf(radPitch);

		// 4. 회전 값 설정 (필요 시)
		// 이 좌표계에서 카메라가 원점을 바라보게 하려면 
		// Yaw에 180도를 더해 반대 방향을 보게 조정합니다.
		GetWorldTransform().SetPosition({ posX , posY, posZ, 1.0f});
		GetWorldTransform().SetRotation({ 0.0f , pitch, yaw + 180.0f, 0.0f });
	}
}

void EditorCamera::BeginPlay()
{
	/*IMesh* pMesh;
	IMaterial* pMaterial;
	MeshManager::Instance()->GetMesh(&pMesh, 11);
	MaterialManager::Instance()->GetMaterial(&pMaterial, 10);
	pStaticMesh_->InitPSO(1, 1, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pStaticMesh_->GetPSO()->SetMeshToSlot(0, pMesh);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(0, pMaterial);*/

	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void EditorCamera::Render(bool isFirst)
{
	Camera::Render(isFirst);
}

void EditorCamera::CleanUp()
{
}
