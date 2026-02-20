#pragma once

struct Bone 
{
	std::string name;
	int parentIndex;
	Float4x4 globalBindPose;
	Float4x4 localBindPose;
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

	const std::vector<Bone>& GetBones() const
	{
		return bones_;
	}

private:
	ULONG refCount_;

	std::vector<Bone> bones_;
};