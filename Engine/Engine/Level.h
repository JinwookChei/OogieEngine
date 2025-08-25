#pragma once

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
	ActorType* SpawnActor()
	{
		ActorType* newActor = new ActorType();

		SpawnActorInternal(newActor);

		return newActor;
	}

private:
	void UpdateTick(double deltaTime);

	void SpawnActorInternal(Actor* pActor);

	void RegisterActor(Actor* pActor);

	void CleanUp(); 

	LINK_ITEM* actorHead_;

	LINK_ITEM* actorTail_;
};

