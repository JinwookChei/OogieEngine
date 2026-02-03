#pragma once

struct IFBXImporter;
class FBXManager
{
private:
	friend class Engine;
	static HMODULE GFbxImporterModule;
	static IFBXImporter* GFbxImporter;
	static bool Init();
	static bool LoadFBXImporter();
	
public:
	static bool LoadModel(Model* pOutModel, const std::string& file);
	static void CleanUp();
};
