#pragma once

struct BoneKeyframe
{
	double time;
	Float4x4 localTransform;
	Float4x4 globalTransform;
};

struct BoneAnimation
{
	std::string boneName;
	std::vector<BoneKeyframe> keyframes;
};

struct AnimationClip
{
	std::string name;
	bool bLoop;
	double duration;     // seconds
	double frameRate;

	std::vector<BoneAnimation> boneAnimations; // bone index 기준
};

struct AnimationPlayState
{
	AnimationPlayState()
		: frameTime(0.0)
		, isBlending(false)
		, isEnd(false)
	{
	}

	void Init()
	{
		frameTime = 0.0;
		isBlending = false;
		isEnd = false;
	}

	double frameTime;

	bool isBlending;
	
	bool isEnd;
};


class Animation : public IUnknown
{
public:
	friend class FBXImporter;
	friend class AnimationManager;
	Animation()
		: refCount_(1)
		, animationClip_()
	{
	}

	~Animation()
	{
	}

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override
	{
		return E_NOTIMPL;
	}

	ULONG __stdcall AddRef() override
	{
		return ++refCount_;
	}

	ULONG __stdcall Release() override
	{
		--refCount_;
		ULONG tmpRefCount = refCount_;
		if (0 == refCount_)
		{
			delete this;
		}
		return tmpRefCount;
	}

	void UpdateAnimation(std::vector<Float4x4>& curAnimBoneMatrices, AnimationPlayState& playState, const Skeleton& srcSkeleton, double deltaTime)
	{
		playState.frameTime += deltaTime;
		playState.isEnd = false;

		// 1. 시간 진행
		if (animationClip_.bLoop)
		{
			while (playState.frameTime > animationClip_.duration)
			{
				playState.frameTime -= animationClip_.duration;
			}
		}
		else
		{
			if (playState.frameTime > animationClip_.duration)
			{
				playState.frameTime = animationClip_.duration;
				playState.isEnd = true;
			}
		}

		const int boneCount = srcSkeleton.GetBoneCount();

		// 2. 각 본에 대해 애니메이션 행렬 계산
		for (int i = 0; i < boneCount; ++i)
		{
			const BoneAnimation& boneAnim = animationClip_.boneAnimations[i];

			// Keyframe 2개 찾기 (Nearest Sampling)
			const BoneKeyframe* k0 = nullptr;
			const BoneKeyframe* k1 = nullptr;

			for (size_t k = 0; k + 1 < boneAnim.keyframes.size(); ++k)
			{
				if (
					playState.frameTime >= boneAnim.keyframes[k].time
					&& playState.frameTime <= boneAnim.keyframes[k + 1].time
					)
				{
					k0 = &boneAnim.keyframes[k];
					k1 = &boneAnim.keyframes[k + 1];
					break;
				}
			}

			if (!k0 || !k1)
			{
				continue;
			}

			double t0 = k0->time;
			double t1 = k1->time;
			double delta = (playState.frameTime - t0) / (t1 - t0);

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
			Float4 scale_delta_sub_tBtA;
			MATH::VectorScale(scale_delta_sub_tBtA, sub_tBtA, delta);
			Float4 T;
			MATH::VectorAdd(T, tA, scale_delta_sub_tBtA);
			Float4 Q = MATH::VectorSlerp(rA, rB, delta);

			Float4 sub_sBsA;
			MATH::VectorSub(sub_sBsA, sB, sA);
			Float4 scale_delta_sub_sBsA;
			MATH::VectorScale(scale_delta_sub_sBsA, sub_sBsA, delta);
			Float4 S;
			MATH::VectorAdd(S, sA, scale_delta_sub_sBsA);

			Float4x4 animatedGlobal;
			MATH::MatrixCompose(animatedGlobal, S, Q, T);

			// 4. Skinning 최종 행렬
			MATH::MatrixMultiply(curAnimBoneMatrices[i], srcSkeleton.GetBones(i).invGlobalBindPose, animatedGlobal);
		}
	}

	inline bool BlendAnimation(std::vector<Float4x4>& curAnimBoneMats, std::vector<Float4x4>& curAnimBoneLocalMats, Skeleton* pSrcSkeleton, AnimationPlayState& playState, double transTime, double deltaTime)
	{
		playState.frameTime += deltaTime;
		if (playState.frameTime > transTime)
		{
			return true;
		}

		const int boneCount = pSrcSkeleton->GetBoneCount();
		for (int bone = 0; bone < boneCount; ++bone)
		{
			const BoneAnimation& boneAnim = animationClip_.boneAnimations[bone];
			const BoneKeyframe* kf = nullptr;

			for (size_t k = 0; k + 1 < boneAnim.keyframes.size(); ++k)
			{
				if (playState.frameTime >= boneAnim.keyframes[k].time && playState.frameTime <= boneAnim.keyframes[k + 1].time)
				{
					kf = &boneAnim.keyframes[k];
					break;
				}
			}

			if (!kf)
			{
				continue;
			}
			
			double delta = playState.frameTime / transTime;

			Float4 sA;
			Float4 rA;
			Float4 tA;
			MATH::MatrixDecompose(sA, rA, tA, curAnimBoneLocalMats[bone]);

			Float4 sB;
			Float4 rB;
			Float4 tB;
			MATH::MatrixDecompose(sB, rB, tB, kf->globalTransform);

			Float4 sub_tBtA;
			MATH::VectorSub(sub_tBtA, tB, tA);
			Float4 scale_delta_sub_tBtA;
			MATH::VectorScale(scale_delta_sub_tBtA, sub_tBtA, delta);
			Float4 T;
			MATH::VectorAdd(T, tA, scale_delta_sub_tBtA);
			Float4 Q = MATH::VectorSlerp(rA, rB, delta);

			Float4 sub_sBsA;
			MATH::VectorSub(sub_sBsA, sB, sA);
			Float4 scale_delta_sub_sBsA;
			MATH::VectorScale(scale_delta_sub_sBsA, sub_sBsA, delta);
			Float4 S;
			MATH::VectorAdd(S, sA, scale_delta_sub_sBsA);

			Float4x4 animatedGlobal;
			//MATH::MatrixComposeQuat(animatedGlobal, S, Q, T);
			MATH::MatrixCompose(animatedGlobal, S, Q, T);

			MATH::MatrixMultiply(curAnimBoneMats[bone], pSrcSkeleton->GetBones(bone).invGlobalBindPose, animatedGlobal);
		}

		return false;
	}


	int SearchCloseAnimationFrame(std::vector<Float4x4>& curAnimBoneLocalMats, Skeleton* pSrcSkeleton, float rotWeight = 1, float posWeight = 0.25)
	{
		std::vector<BoneAnimation>& destAnimations = animationClip_.boneAnimations;
		int boneCount = pSrcSkeleton->GetBoneCount();

		int closeFrame = 0;
		double minDiff = 0x7FEFFFFFFFFFFFFF;

		int searchRange;
		if (animationClip_.bLoop)
		{
			searchRange = destAnimations[0].keyframes.size();
		}
		else
		{
			searchRange = destAnimations[0].keyframes.size() / 2;
		}

		for (int frame = 0; frame < searchRange; ++frame)
		{
			double diff = 0;
			for (int bone = 0; bone < boneCount; ++bone)
			{
				diff += CalcBoneDifference(curAnimBoneLocalMats[bone], destAnimations[bone].keyframes[frame].localTransform, rotWeight, posWeight);
			}

			if (diff < minDiff)
			{
				closeFrame = frame;
				minDiff = diff;
			}
		}
		
		return closeFrame;
	}

	inline double CalcBoneDifference(Float4x4& srcAnimBoneMat, Float4x4& dstAnimBoneMat, float rotWeight, float posWeight)
	{
		double diff = 0.0;

		Float4 srcS;
		Float4 srcQ;
		Float4 srcT;
		MATH::MatrixDecompose(srcS, srcQ, srcT, srcAnimBoneMat);

		Float4 dstS;
		Float4 dstQ;
		Float4 dstT;
		MATH::MatrixDecompose(dstS, dstQ, dstT, dstAnimBoneMat);

		float rotDiff;
		float srcQ_dot_destQ;
		MATH::VectorDot(srcQ_dot_destQ, srcQ, dstQ);
		rotDiff = 1.0f - abs(srcQ_dot_destQ);

		float posDiff;
		Float4 srcT_sub_dstT;
		MATH::VectorSub(srcT_sub_dstT, srcT, dstT);
		MATH::VectorLength(posDiff, srcT_sub_dstT);

		diff = rotWeight * rotDiff + posWeight * posDiff;
		return diff;
	}


	AnimationClip& GetAnimationClip()
	{
		return animationClip_;
	}

private:
	ULONG refCount_;

	AnimationClip animationClip_;
};