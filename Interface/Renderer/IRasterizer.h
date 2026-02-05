#pragma once

enum class E_FILLMODE_TYPE
{
	SOLID = 0,
	WIREFRAME
};


struct IRasterizer : public IUnknown {
	virtual void __stdcall Setting() = 0;

	virtual void __stdcall ChangeFillMode(E_FILLMODE_TYPE fillModeType) = 0;
};