#include "stdafx.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"

SkeletalMeshComponent::SkeletalMeshComponent()
	: pSkeleton_(nullptr)
	, pCurAnimation_(nullptr)
	, pNextAnimation_(nullptr)
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
	AnimationTick(deltaTime);
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

void SkeletalMeshComponent::AnimationTick(double deltaTime)
{
	if (nullptr != pCurAnimation_)
	{
		if (nullptr != pNextAnimation_)
		{
			if (true == pCurAnimation_->Transition(curAnimBoneMatrices_, pNextAnimation_, pSkeleton_, animPlayState_, 0.1, deltaTime))
			{
				pCurAnimation_->Release();
				pCurAnimation_ = pNextAnimation_;
				pNextAnimation_ = nullptr;
				animPlayState_.Init();
			}
		}
		else
		{
			pCurAnimation_->Update(curAnimBoneMatrices_, animPlayState_, *pSkeleton_, deltaTime);
		}
	}
}

bool SkeletalMeshComponent::ChangeAnimation(unsigned long long animTag)
{
	if (nullptr == pCurAnimation_)
	{
		if (false == AnimationManager::Instance()->GetAnimation(&pCurAnimation_, animTag))
		{
			DEBUG_BREAK();
			return false;
		}
		pCurAnimation_->AddRef();
		animPlayState_.Init();
		return true;
	}
	else
	{
		if (nullptr != pNextAnimation_)
		{
			pNextAnimation_->Release();
			pNextAnimation_ = nullptr;
		}

		if (false == AnimationManager::Instance()->GetAnimation(&pNextAnimation_, animTag))
		{
			DEBUG_BREAK();
			return false;
		}
		pNextAnimation_->AddRef();
		animPlayState_.Init();
	}
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
	if (nullptr != pNextAnimation_)
	{
		pNextAnimation_->Release();
		pNextAnimation_ = nullptr;
	}
	if (nullptr != pSkeleton_)
	{
		pSkeleton_->Release();
		pSkeleton_ = nullptr;
	}
}

