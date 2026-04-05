#pragma once

class Actor;
class EditorCamera;
class Light;

class Level
{
public:
	friend class World;

	ENGINE_API Level();
	ENGINE_API virtual ~Level();
	ENGINE_API virtual void BeginPlay();
	ENGINE_API virtual void Tick(double deltaTime);

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
		SpawnActorInternal(newCamera, E_ACTOR_TYPE::CAMERA);
		return newCamera;
	}

	template <typename LightType>
	LightType* SpawnLight()
	{
		LightType* newLight = new LightType;
		SpawnActorInternal(newLight, E_ACTOR_TYPE::LIGHT);
		return newLight;
	}

	LinkedList* GetActorList(const E_ACTOR_TYPE& actoryType);

private:
	ENGINE_API void SpawnActorInternal(Actor* pActor, E_ACTOR_TYPE actorType);
	void RegisterActor(Actor* pActor);

private:
	void OnTick(double deltaTime);
	void OnRender();
	void OnActorTick(double deltaTime);
	void OnRenderActors();
	void OnRenderLights(IRenderTarget* pGBufferTarget);
	void OnRenderParticles();
	void BlitCameraToBackBuffer();
	void CleanUp(); 
	void CleanUpActors();

	LinkedList actorList_[(int)E_ACTOR_TYPE::MAX];
	EditorCamera* pEditorCamera_;
};


