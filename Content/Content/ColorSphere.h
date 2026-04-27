#pragma once

class ColorSphere
	: public Actor
{
public:
	ColorSphere();
	~ColorSphere() override;

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
