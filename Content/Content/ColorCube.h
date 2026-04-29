#pragma once

class ColorCube
	: public Actor
{
public:
	ColorCube();
	~ColorCube() override;

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
