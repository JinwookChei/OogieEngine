#pragma once

enum class E_ACTOR_TYPE
{
	NONE = 0,
	NORMAL,
	LIGHT,
	CAMERA,
	MAX,
};


class Actor;
class Camera;
class Light;

class Level
{
public:
	friend class World;

	ENGINE_API Level();

	ENGINE_API virtual ~Level();

	ENGINE_API virtual void BeginPlay() = 0;

	ENGINE_API virtual void Tick(double deltaTime) = 0;

	template <typename ActorType>
	ActorType* SpawnActor(E_ACTOR_TYPE actorType)
	{
		ActorType* newActor = new ActorType();

		SpawnActorInternal(newActor, actorType);

		return newActor;
	}

	template <typename CameraType>
	CameraType* SpawnCamera()
	{
		CameraType* newCamera = new CameraType;

		SpawnCameraInternal(newCamera);

		return newCamera;
	}

	template <typename LightType>
	LightType* SpawnLight()
	{
		LightType* newLight = new LightType;

		SpawnLightInternal(newLight);

		return newLight;
	}

	//LinkedList* GetActorList() const;

	LinkedList* GetActorList(const E_ACTOR_TYPE& actoryType);

private:
	ENGINE_API void SpawnActorInternal(Actor* pActor, E_ACTOR_TYPE actorType);

	ENGINE_API void SpawnCameraInternal(Camera* pCamera);

	ENGINE_API void SpawnLightInternal(Light* pLight);

	void RegisterActor(Actor* pActor, E_ACTOR_TYPE actorType);

private:
	void OnTick(double deltaTime);

	void OnRender();

	void OnActorTick(double deltaTime);

	void OnRenderActors();

	void OnLightPass();
	
	void BlitCameraToBackBuffer();

	void CleanUp(); 
	
	void CleanUpActors();

	//LinkedList* pActorList_;
	LinkedList actorList_[(int)E_ACTOR_TYPE::MAX];

	//LinkedList particleList_;
};


