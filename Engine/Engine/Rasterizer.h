#pragma once

class Rasterizer
{
public:
	Rasterizer();

	Rasterizer(IRasterizer* pRasterizer);

	virtual ~Rasterizer();

	void Setting();

	void SetFillMode(FillModeType fillModeType);

	void SetRasterizer(IRasterizer* pRasterizer);

private:
	void CleanUp();

	IRasterizer* pRasterizerImpl_;
};
