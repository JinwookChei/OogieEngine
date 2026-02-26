#include "stdafx.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"

SkeletalMeshComponent::SkeletalMeshComponent()
	:  pSkeleton_(nullptr)
	, pCurAnimation_(nullptr)
	, animPlayState_()
	, curAnimBoneMatrices_()
{
}

SkeletalMeshComponent::~SkeletalMeshComponent()
{
	CleanUp();
}

void SkeletalMeshComponent::BeginPlay()
{
}

void SkeletalMeshComponent::Tick(double deltaTime)
{
	MeshComponent::Tick(deltaTime);

	if(nullptr != pCurAnimation_)
	{
		pCurAnimation_->Update(curAnimBoneMatrices_, animPlayState_, *pSkeleton_, deltaTime);
	}
}

void SkeletalMeshComponent::Render()
{
	MeshComponent::Render();

	// 임시. StructBuffer로 바꿔야 함.
	AnimConstantBuffer cb;
	for (int i = 0; i < curAnimBoneMatrices_.size(); ++i)
	{
		cb.animTransform[i] = curAnimBoneMatrices_[i];
	}

	Renderer::Instance()->UpdateAnimationFrame(cb);
	ObjectFrameData objectFrameData;
	MATH::MatrixMultiply(objectFrameData.worldMatrix, GetComponentTransform().GetAffineMatrix(), GetOwner()->GetWorldTransform().GetAffineMatrix());
	Renderer::Instance()->UpdateObjectFrame(objectFrameData);
	Renderer::Instance()->Render(pPSO_);
}



bool SkeletalMeshComponent::ChangeAnimation(unsigned long long animTag)
{
	if (nullptr != pCurAnimation_)
	{
		pCurAnimation_->Release();
		pCurAnimation_ = nullptr;
	}

	if (false == AnimationManager::Instance()->GetAnimation(&pCurAnimation_, animTag))
	{
		DEBUG_BREAK();
		return false;
	}
	pCurAnimation_->AddRef();

	animPlayState_.frameTime = 0.0;
	animPlayState_.isEnd = false;

	return true;
}

bool SkeletalMeshComponent::SetSkeleton(unsigned long long skeletonTag)
{
	if (nullptr != pSkeleton_)
	{
		pSkeleton_->Release();
		pSkeleton_ = nullptr;
	}

	if (false == SkeletonManager::Instance()->GetSkeleton(&pSkeleton_, skeletonTag))
	{
		DEBUG_BREAK();
		return false;
	}
	pSkeleton_->AddRef();
	curAnimBoneMatrices_.resize(pSkeleton_->GetBoneCount());

	return true;
}

void SkeletalMeshComponent::CleanUp()
{
	if (nullptr != pCurAnimation_)
	{
		pCurAnimation_->Release();
		pCurAnimation_ = nullptr;
	}

	if (nullptr != pSkeleton_)
	{
		pSkeleton_->Release();
		pSkeleton_ = nullptr;
	}
}

