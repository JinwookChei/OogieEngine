#pragma once

class RenderComponent;
class Actor
{
public:
	friend class Level;

	ENGINE_API Actor();

	ENGINE_API virtual ~Actor();

	ENGINE_API virtual void Tick(double deltaTime) = 0;

	ENGINE_API virtual void BeginPlay() = 0;

	LINK_ITEM* LevelLink();

private:
	ENGINE_API void CleanUp();

	void Render();

protected:
	RenderComponent* pRenderComponent_;

	LINK_ITEM levelLink_;
};
