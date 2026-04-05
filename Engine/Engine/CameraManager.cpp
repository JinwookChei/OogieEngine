#include "stdafx.h"
#include "RunTimeMode.h"
#include "CameraManager.h"

CameraManager* CameraManager::GCameraManager = nullptr;

CameraManager::CameraManager()
	: pEditorCamera_(nullptr)
	, pPlayerCamera_(nullptr)
{
}

CameraManager::~CameraManager()
{
}

bool CameraManager::Init()
{
	if (nullptr != GCameraManager)
	{
		DEBUG_BREAK();
		return false;
	}

	GCameraManager = new CameraManager;
	return true;
}

void CameraManager::CleanUp()
{
	if (nullptr != GCameraManager)
	{
		delete GCameraManager;
		GCameraManager = nullptr;
	}
}

Camera* CameraManager::GetCurrentCamera()
{
	if (RunTimeMode::GetCurrentMode() == E_RUNTIME_MODE::EDITOR)
	{
		return GCameraManager->pEditorCamera_;
	}
	else
	{
		return GCameraManager->pPlayerCamera_;
	}
	return nullptr;
}

void CameraManager::SetEditorCamera(Camera* pCamera)
{
	CameraManager::GCameraManager->pEditorCamera_ = pCamera;
	Editor::GetEditor()->BindCamera(GCameraManager->pEditorCamera_);
}

void CameraManager::SetPlayerCamera(Camera* pCamera)
{
	CameraManager::GCameraManager->pPlayerCamera_ = pCamera;
}

