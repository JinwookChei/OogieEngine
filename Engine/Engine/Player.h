#pragma once


class Player : public Actor
{
public:
	ENGINE_API Player();
	ENGINE_API ~Player() override;

	ENGINE_API void Tick(double deltaTime) override;
	ENGINE_API void BeginPlay() override;
	ENGINE_API void Render() override;
private:
	ENGINE_API void CleanUp() override;
};
