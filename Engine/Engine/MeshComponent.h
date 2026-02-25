#pragma once


class MeshComponent
{
public:
	ENGINE_API MeshComponent(Actor* pOwner);
	ENGINE_API virtual ~MeshComponent();

	ENGINE_API virtual void Tick(double deltaTime) = 0;

	ENGINE_API virtual void Render() = 0;

	ENGINE_API void InitPSO
	(
		uint16_t meshSlotCount,
		uint16_t meshMaterialCount,
		E_DEPTH_PRESET depthState,
		E_RASTERIZER_PRESET rasterizerMode
	);

	ENGINE_API IPSO* GetPSO() const;

	// MeshLoad Delegate
public:
	//TEMP
	// using MeshLoadedDelegate = std::function<void(IMesh*)>;
	// void BindOnMeshLoaded(MeshLoadedDelegate callback);

private:
	//TEMP
	// void BroadcastOnMeshLoaded();
	// std::vector<MeshLoadedDelegate> OnMeshLoaded_;
	// MeshLoad Delegate End	

private:
	virtual void CleanUp();

protected:
	Actor* pOwner_;

	IPSO* pPSO_;
};