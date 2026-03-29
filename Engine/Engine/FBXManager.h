#pragma once

struct IFBXImporter;
class FBXManager
{
private:
	friend class Engine;
	static HMODULE GFbxImporterModule;
	static IFBXImporter* GFbxImporter;
	static bool LoadFBXImporter();
	
	static void TestLoad();

public:
	static bool LoadStaticModel(StaticModel* pOutModel, const std::string& file);
	static bool LoadSkeletalModel(SkeletalModel* pOutModel, const std::string& file);
	static bool LoadAnimation(Animation* pOutAnimation, const std::string& file);
	static void ShutDown();
};

