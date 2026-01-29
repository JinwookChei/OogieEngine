#pragma once

struct IFBXImporter : public IUnknown
{
	virtual bool ImportModel(Model* pOutModel, const std::string& file) = 0;
};


