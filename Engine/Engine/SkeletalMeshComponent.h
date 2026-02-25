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

	void UpdateAnimation( double deltaTime );

private:
	void CleanUp() override;

	// 얘네가 여기 있는게 맞나?
	double curTime_;
	bool bLoop_;
	// 

	Animation* pAnimation_;
	
	Skeleton* pSkeleton_;

	std::vector<Float4x4> curAnimBoneMatrices_;
};
 