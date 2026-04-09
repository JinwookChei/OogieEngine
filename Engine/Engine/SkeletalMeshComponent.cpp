#include "stdafx.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"

SkeletalMeshComponent::SkeletalMeshComponent()
	: pSkeleton_(nullptr)
	, pAnimation_(nullptr)
	, animPlayState_()
	, curAnimBoneMats_()
	, tmpAnimBoneLocalMats_()
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
	for (int i = 0; i < curAnimBoneMats_.size(); ++i)
	{
		cb.animTransform[i] = curAnimBoneMats_[i];
	}
	Renderer::Instance()->UpdateAnimationFrame(cb);

	ObjectFrameData objectFrameData;
	MATH::MatrixMultiply(objectFrameData.worldMatrix, GetComponentTransform().GetMatrix(), GetOwner()->GetWorldTransform().GetMatrix());
	objectFrameData.scale = GetWorldScale();
	Renderer::Instance()->UpdateObjectFrame(objectFrameData);
	Renderer::Instance()->Render(pPSO_);
}

void SkeletalMeshComponent::AnimationTick(double deltaTime)
{
	if (nullptr != pAnimation_)
	{
		if (animPlayState_.isBlending)
		{
			if (true == pAnimation_->BlendAnimation(curAnimBoneMats_, tmpAnimBoneLocalMats_, pSkeleton_, animPlayState_, 0.04, deltaTime))
			{
				animPlayState_.isBlending = false;
			}
		}
		else
		{
			pAnimation_->UpdateAnimation(curAnimBoneMats_, animPlayState_, *pSkeleton_, deltaTime);
		}
	}
}

bool SkeletalMeshComponent::ChangeAnimation(unsigned long long animTag)
{
	animPlayState_.Init();

	if (nullptr == pSkeleton_)
	{
		DEBUG_BREAK();
		return false;
	}

	for (int bone = 0; bone < pSkeleton_->GetBoneCount(); ++bone)
	{
		MATH::MatrixMultiply(tmpAnimBoneLocalMats_[bone], pSkeleton_->GetBones(bone).globalBindPose, curAnimBoneMats_[bone]);

	}

	if (nullptr != pAnimation_)
	{
		pAnimation_->Release();
		animPlayState_.isBlending = true;
	}

	if (false == AnimationManager::Instance()->GetAnimation(&pAnimation_, animTag))
	{
		DEBUG_BREAK();
		return false;
	}
	pAnimation_->AddRef();

	
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
	curAnimBoneMats_.resize(pSkeleton_->GetBoneCount());
	tmpAnimBoneLocalMats_.resize(pSkeleton_->GetBoneCount());

	return true;
}

void SkeletalMeshComponent::CleanUp()
{
	if (nullptr != pAnimation_)
	{
		pAnimation_->Release();
		pAnimation_ = nullptr;
	}
	if (nullptr != pSkeleton_)
	{
		pSkeleton_->Release();
		pSkeleton_ = nullptr;
	}
}

