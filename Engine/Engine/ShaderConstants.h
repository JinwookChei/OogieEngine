#pragma once

class ShaderConstants
{
public:
	ShaderConstants();

	ShaderConstants(IConstantBuffer* pConstantBuffer);

	virtual ~ShaderConstants();

	void Update(void* pSrcData);

	void VSSetting(uint32_t slot);

	void PSSetting(uint32_t slot);

	void SetConstantBuffer(IConstantBuffer* pConstantBuffer);

private:
	void CleanUp();

	IConstantBuffer* pConstantBufferImpl_;
};
