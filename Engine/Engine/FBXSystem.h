#pragma once

struct IFBXImporter;
class FBXSystem
{
private:
	friend class Engine;
	static HMODULE GFbxImporterModule;
	static IFBXImporter* GFbxImporter;
	static bool Init();
	static bool LoadFBXImporter();
	static void TestImportFBX();

public:
	static IFBXImporter* GetImporter();
	static void CleanUp();
};
