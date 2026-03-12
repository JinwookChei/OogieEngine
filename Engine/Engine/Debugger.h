#pragma once

class Debugger
{
	friend class Engine;
	Debugger();

	~Debugger();

	static Debugger* GDebugger;

	static Debugger* Create();

	static void ShutDown();
public:
	static void Draw();

	static void DrawDebugLine(const Float3& start, const Float3& end, const Float4& color);
		
	static void DrawDebugRay(const Float3& origin, Float3& dir, float length, const Color& color);
private:
	void CleanUp();

private:
	std::vector<DebugLineVertex> debugLines_;
	IMesh* pDebugLineMesh_;
	IMaterial* pDebugLineMaterial_;
	IPSO* pDebugLinePSO_;
	
};


