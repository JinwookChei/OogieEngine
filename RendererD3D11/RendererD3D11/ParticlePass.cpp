#include "stdafx.h"
#include "Particle.h"
#include "Shader.h"
#include "Texture.h"	
#include "ConstantBuffer.h"
#include "BlendState.h"
#include "SamplerState.h"
#include "ParticlePass.h"

ParticlePass::ParticlePass()
	: refCount_(1)
	, pComputeShader_(nullptr)
	, pRenderShader_(nullptr)
	, pCBPerComputeParticle(nullptr)
	, pCBPerParticle_(nullptr)
	, pBlendState_(nullptr)
	, pSamplerState_(nullptr)
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

void ParticlePass::Tick(IParticle* pParticle,  float deltaTime)
{
	Particle* pParticleImpl = (Particle*)pParticle;
	pParticleImpl->AddAccTime(deltaTime);

	CBPerComputeParticle ccBuffer = {};
	ccBuffer.deltaTime_ = deltaTime;
	ccBuffer.maxParticleNum_ = (UINT)pParticleImpl->GetMaxNumber();
	ccBuffer.accTime_ = pParticleImpl->GetAccTime();
	ccBuffer.patternType_ = (unsigned int)pParticleImpl->GetPatternType();
	pCBPerComputeParticle->Update(&ccBuffer);


	pComputeShader_->Bind();
	pCBPerComputeParticle->BindConstantBufferCS(1);
	pParticleImpl->BindUnorderedAccessViewCS(0);


	UINT groupCount = (pParticleImpl->GetMaxNumber() + 255) / 256;
	GRenderer->DeviceContext()->Dispatch(groupCount, 1, 1);


	pParticleImpl->UnBindUnorderedAccessViewCS(0);
	pComputeShader_->UnBind();
}

void ParticlePass::Render(IParticle* pParticle, const Float4x4& viewProj, const Float3& cameraRight, const Float3& cameraUp)
{
	Particle* pParticleImpl = (Particle*)pParticle;

	pBlendState_->Bind();

	Float4 scale = { 1.0f, 1.0f, 1.0f, 0.0f };
	Float4 rotation = { 0.0f, 0.0f, 0.0f , 0.0f };
	Float4 position = { 0.0f, 0.0f, 0.0f , 1.0f };
	Float4x4 world;
	MATH::MatrixCompose(world, scale, rotation, position);

	CBPerParticle cb = {};
	MATH::MatrixTranspose(cb.world_, world);
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
	pParticleImpl->UnBindShaderResourceViewVS(1);
}


bool ParticlePass::InitShaders()
{
	const wchar_t* pCSPath = L"ParticleCS.cso";
	const wchar_t* pVSPath = L"ParticleVS.cso";
	const wchar_t* pGSPath = L"ParticleGS.cso";
	const wchar_t* pPSPath = L"ParticlePS.cso";
	
	ShaderDesc computeShaderDesc;
	computeShaderDesc.pathCS_ = pCSPath;
	pComputeShader_ = Shader::Create(computeShaderDesc);

	ShaderDesc renderShaderDesc;
	renderShaderDesc.pathVS_ = pVSPath;
	renderShaderDesc.pathGS_ = pGSPath;
	renderShaderDesc.pathPS_ = pPSPath;
	pRenderShader_ = Shader::Create(renderShaderDesc);

	if (nullptr == pComputeShader_ || nullptr == pRenderShader_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool ParticlePass::InitConstantBuffer()
{
	pCBPerComputeParticle = ConstantBuffer::Create(sizeof(CBPerComputeParticle));

	pCBPerParticle_ = ConstantBuffer::Create(sizeof(CBPerParticle));

	if (nullptr == pCBPerComputeParticle || nullptr == pCBPerParticle_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool ParticlePass::InitBlendState()
{
	pBlendState_ = BlendState::Create(E_BLEND_MODE::ALPHA_BLEND);
	
	if (nullptr == pBlendState_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool ParticlePass::InitSamplerState()
{
	pSamplerState_ = SamplerState::Create(E_SAMPLER_TYPE::LINEAR_CLAMP, 0, D3D11_FLOAT32_MAX, 1);
	if (nullptr == pSamplerState_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void ParticlePass::CleanUp()
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

	if (nullptr != pCBPerComputeParticle)
	{
		pCBPerComputeParticle->Release();
		pCBPerComputeParticle = nullptr;
	}

	if (nullptr != pCBPerParticle_)
	{
		pCBPerParticle_->Release();
		pCBPerParticle_ = nullptr;
	}
	
}
