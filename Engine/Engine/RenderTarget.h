#pragma once


class RenderTarget final
{
public:
	RenderTarget();
	virtual ~RenderTarget();

private:
	void CleanUP();

	IRenderTarget* pRenderTargetImpl_;
};
