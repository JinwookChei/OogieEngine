#pragma once

class Actor;
class ActorComponent
{
public:
	ENGINE_API ActorComponent();

	ENGINE_API virtual ~ActorComponent();

	virtual void BeginPlay();

	virtual void Tick(double deltaTime);

	virtual void Render();

	ENGINE_API Actor* GetOwner() const;

	ENGINE_API void SetOwner(Actor* pOwner);

	ENGINE_API void SetEnable(bool bEnable);

	ENGINE_API bool IsEnable() const;
	
private:
	virtual void CleanUp();
	
	Actor* pOwner_;

	bool isEnable_;
};
