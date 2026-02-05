#pragma once

struct TextureDesc
{
	Float2 size_;

	UINT colorData_;
};

struct ITexture : public IUnknown
{
	virtual void __stdcall Setting(UINT slot) = 0;
};