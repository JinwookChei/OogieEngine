#include "stdafx.h"
#include "Particle.h"
#include "Shader.h"
#include "Texture.h"	
#include "ConstantBuffer.h"
#include "BlendState.h"
#include "SamplerState.h"
#include "DepthState.h"
#include "ParticlePass.h"

ParticlePass::ParticlePass()
	: refCount_(1)
	, pComputeShader_(nullptr)
	, pRenderShader_(nullptr)
	, pCBPerComputeParticle_(nullptr)
	, pCBPerParticle_(nullptr)
	, pBlendState_(nullptr)
	, pSamplerState_(nullptr)
	, pDepthState_(nullptr)
{
}

ParticlePass::~ParticlePass()
{
	CleanUp();
}

HRESULT __stdcall ParticlePass::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall ParticlePass::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall ParticlePass::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}

bool ParticlePass::Init()
{
	const wchar_t* pCSPath = L"ParticleCS.cso";
	const wchar_t* pVSPath = L"ParticleVS.cso";
	const wchar_t* pGSPath = L"ParticleGS.cso";
	const wchar_t* pPSPath = L"ParticlePS.cso";

	ShaderDesc computeShaderDesc;
	computeShaderDesc.pathCS_ = pCSPath;
	
	ShaderDesc renderShaderDesc;
	renderShaderDesc.pathVS_ = pVSPath;
	renderShaderDesc.pathGS_ = pGSPath;
	renderShaderDesc.pathPS_ = pPSPath;

	pComputeShader_ = Shader::Create(computeShaderDesc);
	pRenderShader_ = Shader::Create(renderShaderDesc);
	pCBPerComputeParticle_ = ConstantBuffer::Create(sizeof(CBPerComputeParticle));
	pCBPerParticle_ = ConstantBuffer::Create(sizeof(CBPerParticle));
	pBlendState_ = BlendState::Create(E_BLEND_PRESET::ALPHA_BLEND);
	pSamplerState_ = SamplerState::Create(E_SAMPLER_PRESET::LINEAR_CLAMP, 0, D3D11_FLOAT32_MAX, 1);
	pDepthState_ = DepthState::Create(true, false);

	return true;
}

void ParticlePass::Tick(IParticle* pParticle,  float deltaTime)
{
	Particle* pParticleImpl = (Particle*)pParticle;
	pParticleImpl->AddAccTime(deltaTime);

	CBPerComputeParticle ccBuffer = {};
	ccBuffer.deltaTime_ = deltaTime;
	ccBuffer.maxParticleNum_ = (UINT)pParticleImpl->GetMaxNumber();
	ccBuffer.accTime_ = pParticleImpl->GetAccTime();
	ccBuffer.patternType_ = (unsigned int)pParticleImpl->GetPatternType();
	pCBPerComputeParticle_->Update(&ccBuffer);


	pComputeShader_->Bind();
	pCBPerComputeParticle_->BindConstantBufferCS(1);
	pParticleImpl->BindUnorderedAccessViewCS(0);


	UINT groupCount = (pParticleImpl->GetMaxNumber() + 255) / 256;
	GRenderer->DeviceContext()->Dispatch(groupCount, 1, 1);


	pParticleImpl->UnBindUnorderedAccessViewCS(0);
	pComputeShader_->UnBind();
}

void ParticlePass::Render(IParticle* pParticle, const Float4x4 worldTransform,  const Float4x4& viewProj, const Float3& cameraRight, const Float3& cameraUp)
{
	Particle* pParticleImpl = (Particle*)pParticle;

	pBlendState_->Bind();
	pDepthState_->Bind();

	CBPerParticle cb = {};
	MATH::MatrixTranspose(cb.world_, worldTransform);
	MATH::MatrixTranspose(cb.viewProj_, viewProj);
	cb.cameraRight_ = cameraRight;
	cb.startSize_ = 0.5f;
	cb.cameraUp_ = cameraUp;
	cb.endSize_ = 0.1f;
	cb.startColor_ = { 0.1f, 0.6f, 1.0f, 1.0f };
	cb.endColor_ = { 0.1f, 0.1f, 1.0f, 0.0f };
	pCBPerParticle_->Update(&cb);

	
	GRenderer->DeviceContext()->IASetInputLayout(nullptr);
	UINT stride = 0;
	UINT offset = 0;
	ID3D11Buffer* nullVB = nullptr;
	GRenderer->DeviceContext()->IASetVertexBuffers(0, 1, &nullVB, &stride, &offset);
	GRenderer->DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pRenderShader_->Bind();

	pCBPerParticle_->BindConstantBufferGS(0);
	pCBPerParticle_->BindConstantBufferPS(0);

	pParticleImpl->BindShaderResourceViewVS(1);
	pParticleImpl->GetTexture()->BindRenderTextureForPS(0);


	pSamplerState_->BindPS(0);
	
	GRenderer->DeviceContext()->Draw(pParticleImpl->GetMaxNumber(), 0);

	pRenderShader_->UnBind();
	pDepthState_->UnBind();
	pParticleImpl->UnBindShaderResourceViewVS(1);
}


void ParticlePass::CleanUp()
{
	if (nullptr != pDepthState_)
	{
		pDepthState_->Release();
		pDepthState_ = nullptr;
	}
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
	if (nullptr != pComputeShader_)
	{
		pComputeShader_->Release();
		pComputeShader_ = nullptr;
	}

	if (nullptr != pComputeShader_)
	{
		pComputeShader_->Release();
		pComputeShader_ = nullptr;
	}

	if (nullptr != pRenderShader_)
	{
		pRenderShader_->Release();
		pRenderShader_ = nullptr;
	}

	if (nullptr != pCBPerComputeParticle_)
	{
		pCBPerComputeParticle_->Release();
		pCBPerComputeParticle_ = nullptr;
	}

	if (nullptr != pCBPerParticle_)
	{
		pCBPerParticle_->Release();
		pCBPerParticle_ = nullptr;
	}
	
}
