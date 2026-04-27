#pragma once

class ActorComponent;
class Actor 
	: public IEditorBindPickedActor
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

	E_ACTOR_TYPE GetActorType() const;

protected:
	ENGINE_API virtual void CleanUp();

protected:
	E_ACTOR_TYPE actorType_;

	Transform* pTransform_;

	std::unordered_map<std::type_index, ActorComponent*> ownedComponents_;

	LINK_NODE levelLink_;

public:
	ENGINE_API IEditorBindTransform& __stdcall GetTransformForEditor() override;
	ENGINE_API E_ACTOR_TYPE __stdcall GetActorTypeForEditor() const override;

	ENGINE_API int __stdcall GetLightTypeForEditor() const override;
	ENGINE_API Color __stdcall GetDiffuseColorForEditor() const override;
	ENGINE_API Color __stdcall GetSpecularColorForEditor() const override;
	ENGINE_API Color __stdcall GetAmbientColorForEditor() const override;
	ENGINE_API void __stdcall SetDiffuseColorForEditor(const Color& color) override;
	ENGINE_API void __stdcall SetSpecularColorForEditor(const Color& color) override;
	ENGINE_API void __stdcall SetAmbientColorForEditor(const Color& color) override;
	ENGINE_API float __stdcall GetLightIntensity() const override;
	ENGINE_API float __stdcall GetLightRange() const override;
	ENGINE_API float __stdcall GetLightSmooth() const override;
	ENGINE_API float __stdcall GetLightInnerAngle() const override;
	ENGINE_API float __stdcall GetLightOuterAngle() const override;
	ENGINE_API void __stdcall SetLightIntensity(float intensity) override;
	ENGINE_API void __stdcall SetLightRange(float range) override;
	ENGINE_API void __stdcall SetLightSmooth(float smooth) override;
	ENGINE_API void __stdcall SetLightInnerAngle(float innerAngle) override;
	ENGINE_API void __stdcall SetLightOuterAngle(float outerAngle) override;
	ENGINE_API IMaterial* __stdcall GetMaterialForEditor() const override;
};
