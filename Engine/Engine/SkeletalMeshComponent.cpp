#include "stdafx.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* pOwner)
	: MeshComponent(pOwner)
	, curTime_(0)
	, bLoop_(true)
	, pAnimation_(nullptr)
	, pSkeleton_(nullptr)
	, curAnimBoneMatrices_()
{
}

SkeletalMeshComponent::~SkeletalMeshComponent()
{
	CleanUp();
}

void SkeletalMeshComponent::Tick(double deltaTime)
{
	UpdateAnimation(deltaTime);
}

void SkeletalMeshComponent::Render()
{
	// 임시. StructBuffer로 바꿔야 함.
	AnimConstantBuffer cb;
	for (int i = 0; i < curAnimBoneMatrices_.size(); ++i)
	{
		cb.animTransform[i] = curAnimBoneMatrices_[i];
	}

	Renderer::Instance()->UpdateAnimationFrame(cb);
	ObjectFrameData objectFrameData;
	objectFrameData.worldMatrix = pOwner_->GetWorldTransform().GetWorldMatrix();
	Renderer::Instance()->UpdateObjectFrame(objectFrameData);
	Renderer::Instance()->Render(pPSO_);
}

bool SkeletalMeshComponent::ChangeAnimation(unsigned long long animTag)
{
	if (nullptr != pAnimation_)
	{
		pAnimation_->Release();
		pAnimation_ = nullptr;
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
	curAnimBoneMatrices_.resize(pSkeleton_->GetBones().size());

	return true;
}

void SkeletalMeshComponent::UpdateAnimation(double deltaTime)
{
	if (nullptr == pAnimation_)
	{
		return;
	}

	const AnimationClip& clip = pAnimation_->GetAnimationClip();

	// 1. 시간 진행
	curTime_ += deltaTime;
	if (bLoop_)
	{
		while (curTime_ > clip.duration)
		{
			curTime_ -= clip.duration;
		}
	}
	else
	{
		if (curTime_ > clip.duration)
		{
			curTime_ = clip.duration;
		}
	}
	
	const int boneCount = pSkeleton_->GetBones().size();

	// 2. 각 본에 대해 애니메이션 행렬 계산
	for (int i = 0; i < boneCount; ++i)
	{
		const BoneAnimation& boneAnim = clip.boneAnimations[i];

		// Keyframe 2개 찾기 (Nearest Sampling)
		const BoneKeyframe* k0 = nullptr;
		const BoneKeyframe* k1 = nullptr;

		for (size_t k = 0; k + 1 < boneAnim.keyframes.size(); ++k)
		{
			if (
				curTime_ >= boneAnim.keyframes[k].time
				&& curTime_ <= boneAnim.keyframes[k+1].time
				)
			{
				k0 = &boneAnim.keyframes[k];
				k1 = &boneAnim.keyframes[k + 1];
				break;
			}
		}

		// fallback
		if (!k0 || !k1)
		{
			continue;
		}

		double t0 = k0->time;
		double t1 = k1->time;
		double alpha = (curTime_ - t0) / (t1 - t0);

		// 3. Global Transform 보간
		Float4 sA;
		Float4 rA;
		Float4 tA;
		MATH::MatrixDecompose(sA, rA, tA, k0->globalTransform);

		Float4 sB;
		Float4 rB;
		Float4 tB;
		MATH::MatrixDecompose(sB, rB, tB, k1->globalTransform);

		Float4 sub_tBtA;
		MATH::VectorSub(sub_tBtA, tB, tA);
		Float4 scale_Alpha_sub_tBtA;
		MATH::VectorScale(scale_Alpha_sub_tBtA, sub_tBtA, alpha);
		Float4 T;
		MATH::VectorAdd(T, tA, scale_Alpha_sub_tBtA);
		Float4 Q = MATH::VectorSlerp(rA, rB, alpha);	

		Float4 sub_sBsA;
		MATH::VectorSub(sub_sBsA, sB, sA);
		Float4 scale_Alpha_sub_sBsA;
		MATH::VectorScale(scale_Alpha_sub_sBsA, sub_sBsA, alpha);
		Float4 S;
		MATH::VectorAdd(S, sA, scale_Alpha_sub_sBsA);

		Float4x4 animatedGlobal;
		MATH::MatrixComposeQuat(animatedGlobal, S, Q, T);		

		// 4. Skinning 최종 행렬
		Float4x4 boneBindPoseInverse;
		MATH::MatrixInverse(boneBindPoseInverse, pSkeleton_->GetBones()[i].globalBindPose);
		MATH::MatrixMultiply(curAnimBoneMatrices_[i], boneBindPoseInverse, animatedGlobal);
	}
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

