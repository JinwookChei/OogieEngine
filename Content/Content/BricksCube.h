#pragma once

class BricksCube
	: public Actor
{
public:
	BricksCube();
	~BricksCube() override;

	void BeginPlay() override;
	void Tick(double deltaTime) override;
	void Render() override;

protected:
	void CleanUp() override;

private:
	StaticMeshComponent* pStaticMesh_;

public:
	IMaterial* __stdcall GetMaterialForEditor() const override;
};
