#pragma once


class MeshComponent 
	: public SceneComponent
{
public:
	ENGINE_API MeshComponent();
	ENGINE_API ~MeshComponent() override;

	void BeginPlay() override;

	void Tick(double deltaTime) override;

	void Render() override;

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
	void CleanUp() override;

protected:
	IPSO* pPSO_;
};