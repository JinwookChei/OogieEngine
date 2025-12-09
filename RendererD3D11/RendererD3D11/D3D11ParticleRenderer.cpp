#include "stdafx.h"
#include "Particle.h"
#include "Texture.h"	
#include "D3D11ParticleRenderer.h"

ParticleSystemGPU::ParticleSystemGPU()
	: refCount_(1)
	, pVertexShader_(nullptr)
	, pGeometryShader_(nullptr)
	, pPixelShader_(nullptr)
	, pComputeShader_(nullptr)
	, pConstantBuffer_(nullptr)
	, pComputeConstantBuffer_(nullptr)
	, pBlendState_(nullptr)
	, pSamplerState_(nullptr)
{
}

ParticleSystemGPU::~ParticleSystemGPU()
{
	CleanUp();
}

HRESULT __stdcall ParticleSystemGPU::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall ParticleSystemGPU::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall ParticleSystemGPU::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}

void __stdcall ParticleSystemGPU::OnTick(IParticle* pParticle, double deltaTime)
{
	Tick(pParticle, deltaTime);
}

void __stdcall ParticleSystemGPU::OnRender(IParticle* pParticle, const Float4x4& viewProj, const Float3& cameraRight, const Float3& cameraUp)
{
	Render(pParticle, viewProj, cameraRight, cameraUp);
}

bool ParticleSystemGPU::Init()
{
	if (false == InitShaders())
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == InitConstantBuffer())
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == InitBlendState())
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == InitSamplerState())
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void ParticleSystemGPU::Tick(IParticle* pParticle,  float deltaTime)
{
	Particle* pParticleImpl = (Particle*)pParticle;
	pParticleImpl->AddAccTime(deltaTime);
	ID3D11UnorderedAccessView* pParticleUAV = pParticleImpl->GetParticlesUAV();

	CBParticleCompute ccBuffer = {};
	ccBuffer.deltaTime_ = deltaTime;
	ccBuffer.maxParticleNum_ = (UINT)pParticleImpl->GetMaxNumber();
	ccBuffer.accTime_ = pParticleImpl->GetAccTime();
	ccBuffer.patternType_ = (unsigned int)pParticleImpl->GetPatternType();
	GRenderer->DeviceContext()->UpdateSubresource(pComputeConstantBuffer_, 0, nullptr, &ccBuffer, 0, 0);


	GRenderer->DeviceContext()->CSSetShader(pComputeShader_, nullptr, 0);
	GRenderer->DeviceContext()->CSSetConstantBuffers(1, 1, &pComputeConstantBuffer_);
	GRenderer->DeviceContext()->CSSetUnorderedAccessViews(0, 1, &pParticleUAV, nullptr);


	UINT groupCount = (pParticleImpl->GetMaxNumber() + 255) / 256;
	GRenderer->DeviceContext()->Dispatch(groupCount, 1, 1);


	ID3D11UnorderedAccessView* nullUAV = nullptr;
	GRenderer->DeviceContext()->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
	GRenderer->DeviceContext()->CSSetShader(nullptr, nullptr, 0);
}

void ParticleSystemGPU::Render(IParticle* pParticle, const Float4x4& viewProj, const Float3& cameraRight, const Float3& cameraUp)
{
	Particle* pParticleImpl = (Particle*)pParticle;

	Float4 scale = { 1.0f, 1.0f, 1.0f, 0.0f };
	Float4 rotation = { 90.0f, 0.0f, 0.0f , 0.0f };
	Float4 position = { 0.0f, 0.0f, 0.0f , 1.0f };
	Float4x4 world;
	MATH::MatrixCompose(world, scale, rotation, position);

	CBParticle cb = {};
	MATH::MatrixTranspose(cb.world_, world);
	MATH::MatrixTranspose(cb.viewProj_, viewProj);
	cb.cameraRight_ = cameraRight;
	cb.startSize_ = 0.5f;
	cb.cameraUp_ = cameraUp;
	cb.endSize_ = 0.1f;
	cb.startColor_ = { 0.1f, 0.6f, 1.0f, 1.0f };
	cb.endColor_ = { 0.1f, 0.1f, 1.0f, 0.0f };
	GRenderer->DeviceContext()->UpdateSubresource(pConstantBuffer_, 0, nullptr, &cb, 0, 0);


	GRenderer->DeviceContext()->IASetInputLayout(nullptr);
	UINT stride = 0;
	UINT offset = 0;
	ID3D11Buffer* nullVB = nullptr;
	GRenderer->DeviceContext()->IASetVertexBuffers(0, 1, &nullVB, &stride, &offset);
	GRenderer->DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);


	ID3D11ShaderResourceView* pParticleSRV = pParticleImpl->GetParticlesSRV();
	GRenderer->DeviceContext()->VSSetShader(pVertexShader_, nullptr, 0);
	GRenderer->DeviceContext()->VSSetShaderResources(1, 1, &pParticleSRV);


	GRenderer->DeviceContext()->GSSetShader(pGeometryShader_, nullptr, 0);
	GRenderer->DeviceContext()->GSSetConstantBuffers(0, 1, &pConstantBuffer_);


	GRenderer->DeviceContext()->PSSetShader(pPixelShader_, nullptr, 0);
	GRenderer->DeviceContext()->PSSetConstantBuffers(0, 1, &pConstantBuffer_);
	//GRenderer->DeviceContext()->PSSetShaderResources(0, 1, &pParticleTextureSRV_);
	pParticleImpl->GetTexture()->BindRenderTextureForPS(0);
	GRenderer->DeviceContext()->PSSetSamplers(0, 1, &pSamplerState_);


	float blendFactor[4] = { 0, 0, 0, 0 };
	UINT samplerMask = 0xffffffff;
	GRenderer->DeviceContext()->OMSetBlendState(pBlendState_, blendFactor, samplerMask);


	GRenderer->DeviceContext()->Draw(pParticleImpl->GetMaxNumber(), 0);


	GRenderer->DeviceContext()->GSSetShader(nullptr, nullptr, 0);
	ID3D11ShaderResourceView* nullSRV[2] = { nullptr, nullptr };
	GRenderer->DeviceContext()->VSSetShaderResources(1, 1, nullSRV);
}


bool ParticleSystemGPU::InitShaders()
{
	const wchar_t* pCSPath = L"ParticleCS.cso";
	const wchar_t* pVSPath = L"ParticleVS.cso";
	const wchar_t* pPSPath = L"ParticlePS.cso";
	const wchar_t* pGSPath = L"ParticleGS.cso";

	ID3DBlob* pCSBlob = nullptr;
	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pPSBlob = nullptr;
	ID3DBlob* pGSBlob = nullptr;

	do
	{
		if (FAILED(D3DReadFileToBlob(pCSPath, &pCSBlob))) break;
		if (FAILED(D3DReadFileToBlob(pVSPath, &pVSBlob))) break;
		if (FAILED(D3DReadFileToBlob(pPSPath, &pPSBlob))) break;
		if (FAILED(D3DReadFileToBlob(pGSPath, &pGSBlob))) break;
		if (FAILED(GRenderer->Device()->CreateComputeShader(pCSBlob->GetBufferPointer(), pCSBlob->GetBufferSize(), nullptr, &pComputeShader_))) break;
		if (FAILED(GRenderer->Device()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVertexShader_))) break;
		if (FAILED(GRenderer->Device()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPixelShader_))) break;
		if (FAILED(GRenderer->Device()->CreateGeometryShader(pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), nullptr, &pGeometryShader_))) break;
		return true;
	} while (true);


	DEBUG_BREAK();

	if (nullptr != pGeometryShader_)
	{
		pGeometryShader_->Release();
		pGeometryShader_ = nullptr;
	}
	if (nullptr != pPixelShader_)
	{
		pPixelShader_->Release();
		pPixelShader_ = nullptr;
	}
	if (nullptr != pVertexShader_)
	{
		pVertexShader_->Release();
		pVertexShader_ = nullptr;
	}
	if (nullptr != pCSBlob)
	{
		pCSBlob->Release();
		pCSBlob = nullptr;
	}
	if (nullptr != pVSBlob)
	{
		pVSBlob->Release();
		pVSBlob = nullptr;
	}
	if (nullptr != pPSBlob)
	{
		pPSBlob->Release();
		pPSBlob = nullptr;
	}
	if (nullptr != pGSBlob)
	{
		pGSBlob->Release();
		pGSBlob = nullptr;
	}

	return false;
}

bool ParticleSystemGPU::InitConstantBuffer()
{
	D3D11_BUFFER_DESC cDesc = {};
	cDesc.Usage = D3D11_USAGE_DEFAULT;
	cDesc.ByteWidth = sizeof(CBParticle);
	cDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	if (FAILED(GRenderer->Device()->CreateBuffer(&cDesc, nullptr, &pConstantBuffer_)))
	{
		DEBUG_BREAK();
		return false;
	}

	D3D11_BUFFER_DESC ccDesc = {};
	ccDesc.Usage = D3D11_USAGE_DEFAULT;
	ccDesc.ByteWidth = sizeof(CBParticleCompute);
	ccDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	if (FAILED(GRenderer->Device()->CreateBuffer(&ccDesc, nullptr, &pComputeConstantBuffer_)))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool ParticleSystemGPU::InitBlendState()
{
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;

	D3D11_RENDER_TARGET_BLEND_DESC& rt0 = blendDesc.RenderTarget[0];
	rt0.BlendEnable = TRUE;
	rt0.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rt0.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rt0.BlendOp = D3D11_BLEND_OP_ADD;
	rt0.SrcBlendAlpha = D3D11_BLEND_ONE;
	rt0.DestBlendAlpha = D3D11_BLEND_ZERO;
	rt0.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rt0.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(GRenderer->Device()->CreateBlendState(&blendDesc, &pBlendState_)))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool ParticleSystemGPU::InitSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(GRenderer->Device()->CreateSamplerState(&samplerDesc, &pSamplerState_)))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void ParticleSystemGPU::CleanUp()
{
	if (nullptr != pSamplerState_)
	{
		pSamplerState_->Release();
		pSamplerState_ = nullptr;
	}
	if (nullptr != pBlendState_)
	{
		pBlendState_->Release();
		pBlendState_ = nullptr;
	}
	if (nullptr != pComputeConstantBuffer_)
	{
		pComputeConstantBuffer_->Release();
		pComputeConstantBuffer_ = nullptr;
	}
	if (nullptr != pConstantBuffer_)
	{
		pConstantBuffer_->Release();
		pConstantBuffer_ = nullptr;
	}
	if (nullptr != pComputeShader_)
	{
		pComputeShader_->Release();
		pComputeShader_ = nullptr;
	}
	if (nullptr != pPixelShader_)
	{
		pPixelShader_->Release();
		pPixelShader_ = nullptr;
	}
	if (nullptr != pGeometryShader_)
	{
		pGeometryShader_->Release();
		pGeometryShader_ = nullptr;
	}

	if (nullptr != pVertexShader_)
	{
		pVertexShader_->Release();
		pVertexShader_ = nullptr;
	}
}
