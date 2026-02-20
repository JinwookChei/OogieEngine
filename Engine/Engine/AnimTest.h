#pragma once


class ENGINE_API AnimTest
{
public:
	AnimTest();
	~AnimTest();

	void Init(uint64_t animTag);

	void UpdateAnimation
	(
		std::vector<Float4x4>* pOutFinalBoneMatrices,
		MeshInfo& mesh,
		double deltaTime
	);

private:
	void CleanUp();

	double curTime_;

	// 이건 애니메이션 클립 안에 있어야 함.
	bool bLoop_;

	Animation* pCurAnimation_;
};
