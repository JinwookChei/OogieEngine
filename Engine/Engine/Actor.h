#pragma once

class ActorComponent;

class Actor : public IEditorBindPickedActor
{
public:
	friend class Level;

	ENGINE_API Actor();

	ENGINE_API virtual ~Actor();

	ENGINE_API virtual void Tick(double deltaTime);

	ENGINE_API virtual void BeginPlay() = 0;

	ENGINE_API virtual void Render();

	ENGINE_API virtual void ParticleRender();

	ENGINE_API Transform& GetWorldTransform() const;

	template<typename ComponentType, typename... Args>
	ComponentType* CreateComponent(Args&&... args)
	{
		std::type_index typeIndex(typeid(ComponentType));

		// 이미 존재하는지 체크
		auto it = ownedComponents_.find(typeIndex);
		if (it != ownedComponents_.end())
		{
			return static_cast<ComponentType*>(it->second);
		}

		// 생성
		ComponentType* newComponent = new ComponentType(std::forward<Args>(args)...);
		newComponent->SetOwner(this);
		newComponent->BeginPlay();
		ownedComponents_[typeIndex] = newComponent;

		return newComponent;
	}

	template<typename ComponentType>
	ComponentType* GetComponent()
	{
		auto it = ownedComponents_.find(std::type_index(typeid(ComponentType)));
		if (it != ownedComponents_.end())
		{
			return static_cast<ComponentType*>(it->second);
		}
		return nullptr;
	}

	void DeleteComponents();


	LINK_NODE* LevelLink();

	//BoundVolume* GetBoundVolume() const;

	// RenderComponent* GetRenderComponent() const;

protected:
	ENGINE_API virtual void CleanUp();

protected:
	Transform* pTransform_;

	std::unordered_map<std::type_index, ActorComponent*> ownedComponents_;

	//BoundVolume* pBoundVolume;

	LINK_NODE levelLink_;

public:
	ENGINE_API IEditorBindTransform* __stdcall GetTransformForEditor() const override;
};
