#pragma once

class MovementComponent
	: public ActorComponent
{
public:
	ENGINE_API MovementComponent();
	ENGINE_API ~MovementComponent() override;

	void BeginPlay() override;

	void Tick(double deltaTime) override;

	void Render() override;

private:
	void CleanUp() override;

	float velocity_;
};
