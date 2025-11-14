#pragma once


class LookAtCamera
	:public Camera
{
public:
	LookAtCamera();

	~LookAtCamera() override;

	void Tick(double deltaTime) override;

	void BeginPlay() override;

	void Render() override;

	void SetFocusActor(Actor* pActor);
private:
	void CameraTransformUpdate() override;

	void CleanUp() override;

	Actor* pFocusActor_;

};
