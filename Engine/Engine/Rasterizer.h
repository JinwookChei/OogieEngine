#pragma once

class Rasterizer
{
public:
	Rasterizer() = delete;

	Rasterizer(IRasterizer* pRasterizer);

	virtual ~Rasterizer();

	void Setting();

	void SetFillMode(EFillModeType fillModeType);

	void SetRasterizer(IRasterizer* pRasterizer);

private:
	void CleanUp();

	IRasterizer* pRasterizerImpl_;
};
