#pragma once


class BlendStateManager
{
private:
	friend class Engine;
	BlendStateManager();
	~BlendStateManager();

public:
	static BlendStateManager* Instance();

	void Clear();

	void Setting(const E_BLEND_MODE_TYPE& blendType);

private:
	void CleanUp();

	IBlendState* pBlendState_;
};
