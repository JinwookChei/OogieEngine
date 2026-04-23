#pragma once

class Floor
	: public Actor
{
public:
	Floor();
	~Floor() override;

	void BeginPlay() override;
	void Tick(double deltaTime) override;
	void Render() override;

protected:
	void CleanUp() override;

private:
	StaticMeshComponent* pStaticMesh_;
};
