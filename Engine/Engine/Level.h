#pragma once

enum class ACTOR_TYPE
{
	NONE = 0,
	NORMAL,
	CAMERA,
	MAX,
};

struct ActorGroupContainer
{
	ACTOR_TYPE actorType_ = ACTOR_TYPE::NONE;

	LINK_ITEM groupLink_;

	LINK_ITEM* pActorHead_ = nullptr;

	LINK_ITEM* pActorTail_ = nullptr;
};


class Actor;
class Level
{
public:
	friend class World;

	ENGINE_API Level();

	ENGINE_API virtual ~Level();

	ENGINE_API virtual void BeginPlay() = 0;

	ENGINE_API virtual void Tick(double deltaTime) = 0;

	template <typename ActorType>
	ActorType* SpawnActor(ACTOR_TYPE actorType)
	{
		ActorType* newActor = new ActorType();

		SpawnActorInternal(newActor, actorType);

		return newActor;
	}

private:
	ENGINE_API void SpawnActorInternal(Actor* pActor, ACTOR_TYPE actorType);

	void RegisterActor(Actor* pActor, ACTOR_TYPE actorType);

public:

private:
	void OnTick(double deltaTime);

	void OnRender();
	
	void CleanUp(); 

	void CleanUpActorGroup();

	LINK_ITEM* pActorGroupHead_;

	LINK_ITEM* pActorGroupTail_;

};

