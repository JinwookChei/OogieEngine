#pragma once

struct IFBXImporter : public IUnknown
{
	virtual bool __stdcall ImportModel(Model* pOutModel, const std::string& file) = 0;

	virtual bool __stdcall ImportAnimation(Animation* pOutAnimation, const std::string& file, double samplingRate = 1.0 / 60.0) = 0;
};


