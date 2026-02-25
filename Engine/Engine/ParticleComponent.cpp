#include "stdafx.h"
#include "ParticleComponent.h"

ParticleComponent::ParticleComponent()
	: particleNum_(1000)
	, accTime_(0.0)
	, particleType_(0)
	, startSize_(0.5f)
	, endSize_(0.1f)
	, startColor_({ 0.1f, 0.6f, 1.0f, 1.0f })
	, endColor_({ 0.1f, 0.1f, 1.0f, 0.0f })
	, pPSO_(nullptr)
	, pParticle_(nullptr)
{
	std::vector<Particle> particleVertices(1000);
	for (Particle& particle : particleVertices)
	{
		particle.position = { 0.0f,0.0f, 0.0f };
		particle.velocity = { 0.0f,0.0f, 0.0f };
		particle.lifeTime = 1.0f;
		particle.age = 1.0f;
	}

	MeshDesc particleDesc;
	particleDesc.primitiveType = E_MESH_PRIMITIVE_TYPE::Point;
	particleDesc.resourceFlag = E_MESH_RESOURCE_FLAG::ShaderResource | E_MESH_RESOURCE_FLAG::UnorderedAccess;
	particleDesc.vertexFormatSize = sizeof(Particle);
	particleDesc.vertexCount = particleVertices.size();
	particleDesc.pVertices = particleVertices.data();
	particleDesc.meshSubsets;
	pParticle_ = Renderer::GetFactory()->CreateMesh(particleDesc);


	IMaterial* pComputeMaterial = nullptr;
	MaterialManager::Instance()->GetMaterial(&pComputeMaterial, 1);
	IMaterial* pRenderMaterial = nullptr;
	MaterialManager::Instance()->GetMaterial(&pRenderMaterial, 2);

	PipelineStateDesc psoDesc;
	psoDesc.meshSlotCount = 1;
	psoDesc.materialSlotCount = 2;
	psoDesc.depthState = E_DEPTH_PRESET::DEPTH_ENABLE_READ_ONLY;
	psoDesc.rasterizerMode = E_RASTERIZER_PRESET::SOLID;
	pPSO_ = Renderer::GetFactory()->CreatePipelineStateObject(psoDesc);
	pPSO_->SetMeshToSlot(0, pParticle_);
	pPSO_->SetMaterialToSlot(0, pComputeMaterial);
	pPSO_->SetMaterialToSlot(1, pRenderMaterial);
}

ParticleComponent::~ParticleComponent()
{
	CleanUp();
}

void ParticleComponent::BeginPlay()
{
}

void ParticleComponent::Tick(double deltaTime)
{
	accTime_ += deltaTime;
	ComputeParticleData data;
	data.deltaTime = deltaTime;
	data.particleNum = particleNum_;
	data.accTime = accTime_;
	data.particleType = particleType_;
	Renderer::Instance()->UpdateComputeParticleFrame(data);

	UINT groupCountX = (particleNum_ + 255) / 256;
	Renderer::Instance()->Compute(pPSO_, groupCountX, 1, 1);
}

void ParticleComponent::Render()
{
}

void ParticleComponent::RenderParticle()
{
	const Transform& worldForm = GCurrentCamera->GetWorldTransform();
	Vector eye = worldForm.GetPosition();
	Vector to = worldForm.ForwardVector();
	Vector right = worldForm.RightVector();
	Vector up = worldForm.UpVector();

	const Float2& size = GCurrentCamera->GetRenderSize();
	Float4x4 proj;
	MATH::MatrixPerspectiveFovLH(proj, GCurrentCamera->GetFov(), (size.X / size.Y), GCurrentCamera->GetNear(), GCurrentCamera->GetFar());
	Float4x4 view;
	MATH::MatrixLookToLH(view, eye, to, up);
	Float4x4 viewProj;
	MATH::MatrixMultiply(viewProj, view, proj);
	Float3 cameraRight(right.X, right.Y, right.Z);
	Float3 cameraUp(up.X, up.Y, up.Z);

	RenderParticleData renderData;
	renderData.world = GetOwner()->GetWorldTransform().GetWorldMatrix();;
	renderData.viewProj = viewProj;
	renderData.cameraRight = cameraRight;
	renderData.cameraUp = cameraUp;
	renderData.startSize = startSize_;
	renderData.endSize = endSize_;
	renderData.startColor = startColor_;
	renderData.endColor = endColor_;
	Renderer::Instance()->UpdateRenderParticleFrame(renderData);
	Renderer::Instance()->RenderParticle_Test(pPSO_);
}

void ParticleComponent::CleanUp()
{
	if (nullptr != pPSO_)
	{
		pPSO_->Release();
		pPSO_ = nullptr;
	}

	if (nullptr != pParticle_)
	{
		pParticle_->Release();
		pParticle_ = nullptr;
	}
}