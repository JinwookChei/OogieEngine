#pragma once

struct InputDesc
{
	const char* semanticName_;
	uint32_t semanticIndex_;
	uint32_t format_;
	uint32_t inputSlot_;
	bool isInstanceData_;
};

struct ShaderDesc
{
	const wchar_t* pathCS_ = nullptr;
	const wchar_t* pathVS_ = nullptr;
	const wchar_t* pathGS_ = nullptr;
	const wchar_t* pathPS_ = nullptr;

	std::vector<InputDesc> inputDesc_;
};


struct IShader : public IUnknown
{

};