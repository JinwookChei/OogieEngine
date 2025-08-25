#pragma once

class Renderer;
class Actor
{
public:
	friend class Level;

	ENGINE_API Actor();

	ENGINE_API virtual ~Actor();

	ENGINE_API virtual void Tick(unsigned long long deltaTime) = 0;

	ENGINE_API virtual void BeginPlay() = 0;

private:
	ENGINE_API void CleanUp();

	LINK_ITEM* LevelLink();

	Renderer* pRenderer_;

	LINK_ITEM levelLink_;
};
