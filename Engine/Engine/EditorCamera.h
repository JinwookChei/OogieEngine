#pragma once

class EditorCamera
	: public Camera
{
public:
	EditorCamera();
	~EditorCamera() override;

	void Tick(double deltaTime) override;
	void BeginPlay() override;
	void Render() override;

private:
	void CleanUp() override;


	// Orbit 기능.
	Float3 targetPos = { -4.0f, 5.0f, 0.0f }; // 중심 지점
	float radius = 30.0f;                   // 떨어진 거리 (x)
	float yaw = 0.0f;    // 수평 회전각
	float pitch = 35.0f;  // 수직 회전각
	float rotationSpeed = 30.0f; // 초당 회전 각도 (45도)
};
