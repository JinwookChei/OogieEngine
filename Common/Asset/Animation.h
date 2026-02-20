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
	double duration;     // seconds
	double frameRate;

	std::vector<BoneAnimation> boneAnimations; // bone index ±‚¡ÿ
};

class Animation : public IUnknown
{
public:
	friend class FBXImporter;
	friend class AnimationManager;
	Animation()
		: refCount_(1) {}

	~Animation() {}

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

	const AnimationClip& GetAnimationClip() const
	{
		return animationClip_;
	}

private:
	ULONG refCount_;

	AnimationClip animationClip_;
};