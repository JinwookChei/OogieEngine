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

	void SetTarget(Actor* pTarget);
private:
	void CleanUp() override;

	RenderComponent* pRenderer_;

	Actor* pTarget_;

};
