#pragma once

struct Bone 
{
	std::string name;
	int parentIndex;
	Float4x4 globalBindPose;
	Float4x4 invGlobalBindPose;

	Float4x4 localBindPose;
	Float4x4 invLocalBindPose;
};

class Skeleton : public IUnknown
{
public:
	friend class SkeletonManager;
	Skeleton()
		: refCount_(1) {}

	~Skeleton() {}

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

	int GetBoneCount() const
	{
		return bones_.size();
	}

	const Bone& GetBones(int idx) const
	{
		if (GetBoneCount() <= idx)
		{
			DEBUG_BREAK();
		}

		return bones_[idx];
	}

private:
	ULONG refCount_;

	std::vector<Bone> bones_;
};