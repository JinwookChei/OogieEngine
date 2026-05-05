#pragma once

class Tree
	: public Actor
{
public:
	Tree();
	~Tree() override;

	void BeginPlay() override;
	void Tick(double deltaTime) override;
	void Render(bool isFirst) override;

protected:
	void CleanUp() override;

private:
	StaticMeshComponent* pStaticMesh_;
};
