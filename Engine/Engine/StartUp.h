#pragma once
struct IStartup : public IUnknown
{
	virtual bool __stdcall Start() = 0;

	virtual void  __stdcall End() = 0;
};