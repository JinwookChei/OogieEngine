#include "stdafx.h"
//#include "ShaderManager.h"
//
//ShaderManager::ShaderManager()
//	: shaderTable_(16, 8)
//{
//}
//
//ShaderManager::~ShaderManager()
//{
//	CleanUp();
//}
//
//ShaderManager* ShaderManager::Instance()
//{
//	return GShaderManager;
//}
//
//void ShaderManager::TestLoad()
//{
//
//}
//
//
//IShader* ShaderManager::CreateShader(const ShaderDesc& desc, unsigned long long shaderTag)
//{
//	IShader* pNewShader = Renderer::GetFactory()->CreateShader(desc);
//	shaderTable_.Insert(pNewShader, &shaderTag, 8);
//	return pNewShader;
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
