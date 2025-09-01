#include "stdafx.h"
#include "Test.h"
#include "ConstantManager.h"

ConstantManager::ConstantManager()
	:constantBuffer_(new ConstantBuffer)
{
}

ConstantManager::~ConstantManager()
{
	CleanUp();
}

ConstantManager* ConstantManager::Instance()
{
	return GConstantManager;
}

void ConstantManager::Update()
{
	Float4x4 tmpMat;
	MatrixTranspose(tmpMat, GCamera->View());
	constantBuffer_->view = tmpMat;

	MatrixTranspose(tmpMat, GCamera->Projection());
	constantBuffer_->projection = tmpMat;

	// Light
	constantBuffer_->lightColor = GSpotLight->LightColor();
	constantBuffer_->ambientColor = GSpotLight->AmbientColor();
	constantBuffer_->spotPosition = GSpotLight->SpotPosition();
	constantBuffer_->spotDirection = GSpotLight->SpotDirection();
	constantBuffer_->spotRange = GSpotLight->SpotRange();
	constantBuffer_->spotAngle = GSpotLight->SpotAngle();
}

ConstantBuffer* ConstantManager::GetConstantBuffer() const
{
	return constantBuffer_;
}


void ConstantManager::CleanUp()
{
	if (nullptr != constantBuffer_)
	{
		delete constantBuffer_;
		constantBuffer_ = nullptr;
	}
}
