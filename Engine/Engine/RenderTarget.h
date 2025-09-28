#pragma once


class RenderTarget final
{
public:
	RenderTarget() = delete;

	RenderTarget(IRenderTarget* pRenderTargetImpl);

	virtual ~RenderTarget();

	void Clear();

	void Setting();

	void SetClearColor(const Color& cleaColor);
private:
	void CleanUp();

	IRenderTarget* pRenderTargetImpl_;
};
