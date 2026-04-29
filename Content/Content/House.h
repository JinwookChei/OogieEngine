#pragma once

class House
	: public Actor
{
public:
	House();
	~House() override;

	void BeginPlay() override;
	void Tick(double deltaTime) override;
	void Render() override;

protected:
	void CleanUp() override;

private:
	StaticMeshComponent* pStaticMesh_;
};
