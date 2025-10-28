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
	ActorType* SpawnActor(ACTOR_TYPE actorType)
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

private:
	ENGINE_API void SpawnActorInternal(Actor* pActor, ACTOR_TYPE actorType);

	ENGINE_API void SpawnCameraInternal(Camera* pCamera);

	ENGINE_API void SpawnLightInternal(Light* pLight);

	void RegisterActor(Actor* pActor, ACTOR_TYPE actorType);

	void RegisterCamera(Camera* pCamera);

	void RegisterLight(Light* pLight);

private:
	void OnTick(double deltaTime);

	void OnTickCameras(double deltaTime);

	void OnTickLights(double deltaTime);

	void OnTickActors(double deltaTime);

	void OnRender();

	void OnRenderCameras();

	void OnRenderLights();

	void OnRenderActors();
	
	void BlitCameraToBackBuffer();

	void CleanUp(); 

	void CleanUpCamera();

	void CleanUpLight();

	void CleanUpActorGroup();

	LINK_ITEM* pCameraHead_;

	LINK_ITEM* pCameraTail_;

	LINK_ITEM* pLightHead_;

	LINK_ITEM* pLightTail_;

	LINK_ITEM* pActorGroupHead_;

	LINK_ITEM* pActorGroupTail_;

};


