#include "stdafx.h"
#include "MaterialManager.h"


MaterialManager::MaterialManager()
	: materialTable_(16,8)
{
}

MaterialManager::~MaterialManager()
{
	CleanUp();
}

MaterialManager* MaterialManager::Instance()
{
	return GMaterialManager;
}

void MaterialManager::TestLoad()
{
	MaterialDesc matDesc;
	matDesc.VS = L"ObjectVS.cso";
	matDesc.PS = L"DeferredObjectPS.cso";
	matDesc.shineness = 0.7f;
	matDesc.specularColor = { 0.7f, 0.7f, 0.7f };
	
	CreateMaterial(matDesc, 1);
}

IMaterial* MaterialManager::CreateMaterial(const MaterialDesc& desc, unsigned long long materialTag)
{
	IMaterial* pNewMaterial = GRenderer->CreateMaterial(desc);
	materialTable_.Insert(pNewMaterial, &materialTag, 8);

	return pNewMaterial;
}

bool MaterialManager::GetMaterial(IMaterial** ppOutMaterial, unsigned long long materialTag)
{
	unsigned int searchedCount = 0;
	void* pTmpMaterial = nullptr;

	if (false == materialTable_.Search((void**)&pTmpMaterial, &searchedCount, 8, &materialTag, 8))
	{
		Assert("Material Search is Fail!!");
		return false;
	}

	if (searchedCount != 1)
	{
		DEBUG_BREAK();
		return false;
	}

	*ppOutMaterial = static_cast<IMaterial*>(pTmpMaterial);
	return true;
}


void MaterialManager::CleanUp()
{
	HashTableIterator materialTableEnd = materialTable_.end();
	for (HashTableIterator iter = materialTable_.begin(); iter != materialTableEnd;)
	{
		IMaterial* pCurMaterial = static_cast<IMaterial*>(*iter);
		if (nullptr != pCurMaterial)
		{
			pCurMaterial->Release();
			pCurMaterial = nullptr;
		}
		iter = materialTable_.Delete(iter);
	}
}
