#include "stdafx.h"
#include "AnimTest.h"

AnimTest::AnimTest()
	: curTime_(0)
	, pCurAnimation_(nullptr)
	, bLoop_(false)
{
}

AnimTest::~AnimTest()
{
	CleanUp();
}

void AnimTest::Init(uint64_t animTag)
{
	if(false == AnimationManager::Instance()->GetAnimation(&pCurAnimation_, animTag))
	{
		DEBUG_BREAK();
		return;
	}
}

void AnimTest::UpdateAnimation(std::vector<Float4x4>* pOutFinalBoneMatrices, MeshInfo& mesh, double deltaTime)
{
	const AnimationClip& clip = pCurAnimation_->GetAnimationClip();

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

	const int boneCount = mesh.bones.size();
	pOutFinalBoneMatrices->resize(boneCount);

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
				curTime_ >= boneAnim.keyframes[i].time
				&& curTime_ <= boneAnim.keyframes[i].time
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
			MATH::MatrixInverse((*pOutFinalBoneMatrices)[i], mesh.bones[i].globalBindPose);
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

		//FbxVector4 tA = k0->globalTransform.GetT();
		//FbxVector4 sA = k0->globalTransform.GetS();
		//FbxQuaternion rA = k0->globalTransform.GetQ();
		
		//FbxVector4 tB = k1->globalTransform.GetT();
		//FbxQuaternion rB = k1->globalTransform.GetQ();
		//FbxVector4 sB = k1->globalTransform.GetS();

		Float4 sub_tBtA;
		MATH::VectorSub(sub_tBtA, tB, tA);
		Float4 scale_Alpha_sub_tBtA;
		MATH::VectorScale(scale_Alpha_sub_tBtA, sub_tBtA, alpha);
		Float4 T;
		MATH::VectorAdd(T, tA, scale_Alpha_sub_tBtA);
		//FbxVector4 T = tA + (tB - tA) * alpha;
		
		Float4 Q = MATH::VectorSlerp(rA, rB, alpha);		// 체크 필요.
		/*FbxQuaternion Q;
		Q = rA.Slerp(rB, alpha);*/

		Float4 sub_sBsA;
		MATH::VectorSub(sub_sBsA, sB, sA);
		Float4 scale_Alpha_sub_sBsA;
		MATH::VectorScale(scale_Alpha_sub_sBsA, sub_sBsA, alpha);
		Float4 S;
		MATH::VectorAdd(S, sA, scale_Alpha_sub_sBsA);
		//FbxVector4 S = sA + (sB - sA) * alpha;

		Float4x4 animatedGlobal;
		MATH::MatrixComposeQuat(animatedGlobal, S, Q, T);		// 체크 필요.
		//FbxAMatrix animatedGlobal;
		//animatedGlobal.SetTQS(T, Q, S);

		// 4. Skinning 최종 행렬
		Float4x4 boneBindPoseInverse;
		MATH::MatrixInverse(boneBindPoseInverse, mesh.bones[i].globalBindPose);
		MATH::MatrixMultiply((*pOutFinalBoneMatrices)[i], animatedGlobal, boneBindPoseInverse);
		//outFinalBoneMatrices[i] = animatedGlobal * player.bones_[i].boneBindPose.Inverse();
	}
}


void AnimTest::CleanUp()
{

}