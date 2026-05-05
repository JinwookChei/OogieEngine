#pragma once

class EditorCamera
	: public Camera
{
public:
	EditorCamera();
	~EditorCamera() override;

	void Tick(double deltaTime) override;
	void BeginPlay() override;
	void Render(bool isFirst) override;

private:
	void CleanUp() override;


	// Orbit 기능.
	Float3 targetPos = { 0.0f, 0.0f, 7.0f }; // 중심 지점
	float radius = 15.0f;                   // 떨어진 거리 (x)
	float yaw = 0.0f;    // 수평 회전각
	float pitch = 0.0f;  // 수직 회전각
	float rotationSpeed = 40.0f; // 초당 회전 각도 (45도)
};
