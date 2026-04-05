#pragma once

class Camera;
class CameraManager final
{
	friend class Engine;
	CameraManager();
	~CameraManager();

	static bool Init();
	static void CleanUp();
	static CameraManager* GCameraManager;

public:
	static Camera* GetCurrentCamera();
	static void SetEditorCamera(Camera* pCamera);
	static void SetPlayerCamera(Camera* pCamera);

private:
	Camera* pEditorCamera_;
	Camera* pPlayerCamera_;
};
