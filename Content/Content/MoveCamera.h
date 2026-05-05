#pragma once


class MoveCamera
	: public Camera
{
public:
	MoveCamera();

	~MoveCamera() override;

	void Tick(double deltaTime) override;

	void BeginPlay() override;

	void Render(bool isFirst) override;
private:
	void CleanUp() override;

	StaticMeshComponent* pStaticMesh_;
};
