#pragma once

class SkeletalMeshComponent 
	: public MeshComponent
{
public:
	ENGINE_API SkeletalMeshComponent();

	ENGINE_API ~SkeletalMeshComponent() override;

	void BeginPlay() override;

	void Tick(double deltaTime) override;

	void Render() override;

	ENGINE_API bool ChangeAnimation(unsigned long long animTag);

	ENGINE_API bool SetSkeleton(unsigned long long skeletonTag);

private:
	void CleanUp() override;

	Skeleton* pSkeleton_;

	Animation* pCurAnimation_;

	AnimationPlayState animPlayState_;

	std::vector<Float4x4> curAnimBoneMatrices_;
	
};
 