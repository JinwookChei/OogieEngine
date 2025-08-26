#pragma once

class RenderDevice final
{
public:
	RenderDevice();

	virtual ~RenderDevice();


private:
	IRenderer* pRendererInterface_;

};
