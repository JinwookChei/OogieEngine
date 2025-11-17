#pragma once

class RasterizerManager
{
private:
	friend class Engine;

	RasterizerManager();

	~RasterizerManager();

public:
	static RasterizerManager* Instance();

	void Setting(E_FILLMODE_TYPE fillModeType);

private:
	void CleanUp();

	IRasterizer* pRasterizer_;
};
