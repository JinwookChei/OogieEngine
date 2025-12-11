#include "stdafx.h"
#include "Texture.h"
#include "Particle.h"

Particle::Particle()
	:refCount_(1)
	, patternType_(E_PARTICLE_PATTERN_TYPE::EXPLOSION)
	, accTime_(0)
	, maxNum_(0)
	, pParticles_(nullptr)
	, pParticlesSRV_(nullptr)
	, pParticlesUAV_(nullptr)
	, pTexture_(nullptr)
{
}

Particle::~Particle()
{
	CleanUp();
}

IParticle* Particle::Create(const ParticleDesc& desc)
{
	Particle* pNewParticle = new Particle;
	if (false == pNewParticle->Init(desc))
	{
		pNewParticle->Release();
		pNewParticle = nullptr;
		return nullptr;
	}

	return pNewParticle;
}

bool Particle::Init(const ParticleDesc& desc)
{
	if (false == InitParticleBuffer(desc.maxNum_))
	{
		return false;
	}

	if (false == InitTexture(desc.pTexture_))
	{
		return false;
	}

	patternType_ = desc.patternType_;

	return true;
}

HRESULT __stdcall Particle::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall Particle::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall Particle::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}

E_PARTICLE_PATTERN_TYPE Particle::GetPatternType() const
{
	return patternType_;
}

unsigned int Particle::GetMaxNumber() const
{
	return maxNum_;
}

double Particle::GetAccTime() const
{
	return accTime_;
}

void Particle::AddAccTime(double deltaTime)
{
	accTime_ += deltaTime;
}

void Particle::BindUnorderedAccessViewCS(UINT slot)
{
	GRenderer->DeviceContext()->CSSetUnorderedAccessViews(slot, 1, &pParticlesUAV_, nullptr);
}

void Particle::UnBindUnorderedAccessViewCS(UINT slot)
{
	ID3D11UnorderedAccessView* pNullUAV = nullptr;
	GRenderer->DeviceContext()->CSSetUnorderedAccessViews(slot, 1, &pNullUAV, nullptr);
}

void Particle::BindShaderResourceViewVS(UINT slot)
{
	GRenderer->DeviceContext()->VSSetShaderResources(slot, 1, &pParticlesSRV_);
}

void Particle::UnBindShaderResourceViewVS(UINT slot)
{
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	GRenderer->DeviceContext()->VSSetShaderResources(slot, 1, &pNullSRV);
}

void Particle::BindShaderResourceViewPS(UINT slot)
{
	GRenderer->DeviceContext()->PSSetShaderResources(slot, 1, &pParticlesSRV_);
}

void Particle::UnBindShaderResourceViewPS(UINT slot)
{
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	GRenderer->DeviceContext()->PSSetShaderResources(slot, 1, &pNullSRV);
}

//void Particle::BindUnorderedAccessViewCS(UINT slot)
//{
//	GRenderer->DeviceContext()->CSSetUnorderedAccessViews(slot, 1, &pParticlesUAV_, nullptr);
//}

//void Particle::UnBindUnorderedAccessViewCS()
//{
//}

//ID3D11ShaderResourceView* Particle::GetParticlesSRV() const
//{
//	return pParticlesSRV_;
//}
//
//ID3D11UnorderedAccessView* Particle::GetParticlesUAV() const
//{
//	return pParticlesUAV_;
//}
//
Texture* Particle::GetTexture() const
{
	return pTexture_;
}


bool Particle::InitParticleBuffer(unsigned int maxNum)
{
	maxNum_ = maxNum;

	std::vector<ParticleElem> initData(maxNum_);
	for (ParticleElem& particle : initData)
	{
		particle.position_ = { 0.0f, 0.0f, 0.0f };
		particle.velocity_ = { 0.0f, 0.0f, 0.0f };
		particle.lifeTime_ = 1.0f;
		particle.age_ = 1.0f;
	}

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(ParticleElem) * maxNum_;
	bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = sizeof(ParticleElem);

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = initData.data();

	if (FAILED(GRenderer->Device()->CreateBuffer(&bufferDesc, &data, &pParticles_)))
	{
		DEBUG_BREAK();
		return false;
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = maxNum_;

	if (FAILED(GRenderer->Device()->CreateShaderResourceView(pParticles_, &srvDesc, &pParticlesSRV_)))
	{
		DEBUG_BREAK();
		return false;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = maxNum_;
	uavDesc.Buffer.Flags = 0;

	if (FAILED(GRenderer->Device()->CreateUnorderedAccessView(pParticles_, &uavDesc, &pParticlesUAV_)))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool Particle::InitTexture(ITexture* pTexture)
{
	Texture* pCastedTexture = dynamic_cast<Texture*>(pTexture);
	if (nullptr == pCastedTexture)
	{
		Assert("Texture Casting is Fail!!");
		return false;
	}

	pTexture_ = pCastedTexture;
	pTexture_->AddRef();

	return true;
}

void Particle::CleanUp()
{
	if (nullptr != pTexture_)
	{
		pTexture_->Release();
		pTexture_ = nullptr;
	}
	if (nullptr != pParticlesUAV_)
	{
		pParticlesUAV_->Release();
		pParticlesUAV_ = nullptr;
	}
	if (nullptr != pParticlesSRV_)
	{
		pParticlesSRV_->Release();
		pParticlesSRV_ = nullptr;
	}
	if (nullptr != pParticles_)
	{
		pParticles_->Release();
		pParticles_ = nullptr;
	}
}
