#pragma once

class SkeletalMeshComponent : public MeshComponent
{
public:
	ENGINE_API SkeletalMeshComponent(Actor* pOwner);

	ENGINE_API ~SkeletalMeshComponent() override;

	ENGINE_API void Tick(double deltaTime) override;

	ENGINE_API void Render() override;

	ENGINE_API bool ChangeAnimation(unsigned long long animTag);

	ENGINE_API bool SetSkeleton(unsigned long long skeletonTag);

	void UpdateAnimation( double deltaTime );

private:
	virtual void CleanUp();

	// 얘네가 여기 있는게 맞나?
	double curTime_;
	bool bLoop_;
	//

	Animation* pAnimation_;
	
	Skeleton* pSkeleton_;

	std::vector<Float4x4> curAnimBoneMatrices_;
};
 