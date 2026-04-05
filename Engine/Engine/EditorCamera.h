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
};
