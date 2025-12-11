#include "stdafx.h"
#include "ShaderManager.h"

//
//ShaderManager::ShaderManager()
//	:shaderTable_(16, 8)
//{
//
//}
//
//
//ShaderManager::~ShaderManager()
//{
//	CleanUp();
//}
//
//
//ShaderManager* ShaderManager::Instance()
//{
//	return GShaderManager;
//}
//
//void ShaderManager::TestLoad()
//{
//	ShaderDesc deferredShader;
//	//deferredShader.type_ = E_SHADER_TYPE::MATERIAL_SHADER;
//	deferredShader.inputDesc_.push_back({ "POSITION", 0, 6, 0, false });
//	deferredShader.inputDesc_.push_back({ "COLOR", 0, 2, 0, false });
//	deferredShader.inputDesc_.push_back({ "NORMAL", 0, 6, 0, false });
//	deferredShader.inputDesc_.push_back({ "TEXCOORD", 0, 16, 0, false });
//	deferredShader.inputDesc_.push_back({ "TANGENT", 0, 2, 0, false });
//	deferredShader.pathVS_ = L"ObjectVS.cso";
//	deferredShader.pathPS_ = L"DeferredObjectPS.cso";
//	unsigned long long deferredShaderTag = 1;
//	CreateShader(deferredShader, deferredShaderTag);
//
//
//	ShaderDesc lightPassShader;
//	//lightPassShader.type_ = E_SHADER_TYPE::MATERIAL_SHADER;
//	lightPassShader.inputDesc_.push_back({ "POSITION", 0, 16, 0, false });
//	lightPassShader.inputDesc_.push_back({ "TEXCOORD", 0, 16, 0, false });
//	lightPassShader.pathVS_ = L"ScreenMergeVS.cso";
//	lightPassShader.pathPS_ = L"DeferredLightPS.cso";
//	unsigned long long lightPassShaderTag = 2;
//	CreateShader(lightPassShader, lightPassShaderTag);
//
//
//	ShaderDesc debugPassShader;
//	//debugPassShader.type_ = E_SHADER_TYPE::MATERIAL_SHADER;
//	debugPassShader.inputDesc_.push_back({ "POSITION", 0, 16, 0, false });
//	debugPassShader.inputDesc_.push_back({ "TEXCOORD", 0, 16, 0, false });
//	debugPassShader.pathVS_ = L"ScreenMergeVS.cso";
//	debugPassShader.pathPS_ = L"DeferredDebugPS.cso";
//	unsigned long long debugPassShaderTag = 3;
//	CreateShader(debugPassShader, debugPassShaderTag);
//	
//
//	ShaderDesc screenPassShader;
//	//screenPassShader.type_ = E_SHADER_TYPE::MATERIAL_SHADER;
//	screenPassShader.inputDesc_.push_back({ "POSITION", 0, 16, 0, false });
//	screenPassShader.inputDesc_.push_back({ "TEXCOORD", 0, 16, 0, false });
//	screenPassShader.pathVS_ = L"ScreenMergeVS.cso";
//	screenPassShader.pathPS_ = L"ScreenMergePS.cso";
//	unsigned long long screenPassShaderTag = 4;
//	CreateShader(screenPassShader, screenPassShaderTag);
//}
//
//IShader* ShaderManager::CreateShader(const ShaderDesc& desc, unsigned long long shaderTag)
//{
//	IShader* pShader = GRenderer->CreateShader(desc);
//	shaderTable_.Insert(pShader, &shaderTag, 8);
//
//	return pShader;
//}
//
//bool ShaderManager::GetShader(IShader** ppOutShader, unsigned long long shaderTag)
//{
//	unsigned int searchedCount = 0;
//	void* pTmpShader = nullptr;
//
//	if (false == shaderTable_.Search((void**)&pTmpShader, &searchedCount, 8, &shaderTag, 8))
//	{
//		Assert("Shader Search is Fail!!");
//		return false;
//	}
//
//	if (searchedCount != 1)
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	*ppOutShader = static_cast<IShader*>(pTmpShader);
//	return true;
//}
//
//void ShaderManager::CleanUp()
//{
//	HashTableIterator shaderTableEnd = shaderTable_.end();
//	for (HashTableIterator iter = shaderTable_.begin(); iter != shaderTableEnd;)
//	{
//		IShader* pCurShader = static_cast<IShader*>(*iter);
//		if (nullptr != pCurShader)
//		{
//			pCurShader->Release();
//			pCurShader = nullptr;
//		}
//		iter = shaderTable_.Delete(iter);
//	}
//}
