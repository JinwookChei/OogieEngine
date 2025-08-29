#pragma once


class TestActor
	: public Actor
{
public:
	TestActor();

	~TestActor() override;
	
	void BeginPlay() override;

	void Tick(double deltaTime) override;

	void Render() override;

protected:
	void CleanUp() override;

private:
	RenderComponent* pRenderer_;
};
