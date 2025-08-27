#include "stdafx.h"
#include "TestActor.h"

TestActor::TestActor()
{
}

TestActor::~TestActor()
{
}

void TestActor::BeginPlay()
{
	pRenderComponent_->Create();
}

void TestActor::Tick(double deltaTime)
{

}
