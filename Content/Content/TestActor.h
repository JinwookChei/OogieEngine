#pragma once


class TestActor
	: public Actor
{
public:
	TestActor();

	~TestActor() override;
	
	void BeginPlay() override;

	void Tick(double deltaTime) override;

	void Render(bool isFirst) override;

protected:
	void CleanUp() override;

private:
	StaticMeshComponent* pStaticMesh_;
};
