#pragma once

class Tree
	: public Actor
{
public:
	Tree();
	~Tree() override;

	void BeginPlay() override;
	void Tick(double deltaTime) override;
	void Render() override;

protected:
	void CleanUp() override;

private:
	StaticMeshComponent* pStaticMesh_;
};
