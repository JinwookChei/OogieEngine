#include "stdafx.h"
#include "AnimationManager.h"


AnimationManager::AnimationManager()
	: animationTable_(16, 8)
{
}

AnimationManager::~AnimationManager()
{
	CleanUp();
}

AnimationManager* AnimationManager::Instance()
{
	return GAnimationManager;
}

void AnimationManager::TestLoad()
{
	//Animation* animWolfAttack = CreateAnimation("../Resource/Fbx/WereWolf/Animation/Anim_Werewolf_attack1.FBX", 0);
	Animation* animCapoeira = CreateAnimation("../Resource/Fbx/Mixamo/Capoeira.FBX", 0);
}

Animation* AnimationManager::CreateAnimation(const std::string& fileName, unsigned long long animationTag)
{
	Animation* pNewAnimation = new Animation;
	if (false == FBXManager::LoadAnimation(pNewAnimation, fileName))
	{
		DEBUG_BREAK();
		delete pNewAnimation;
		
		return nullptr;
	}
	
	animationTable_.Insert(pNewAnimation, &animationTag, 8);
	return pNewAnimation;
}

bool AnimationManager::GetAnimation(Animation** ppOutAnimation, unsigned long long animationTag)
{
	unsigned int searchedCount = 0;
	Animation* pTmpAnimation = nullptr;

	if (false == animationTable_.Search((void**)&pTmpAnimation, &searchedCount, 8, &animationTag, 8))
	{
		DEBUG_BREAK();
		//Assert("Mesh Search is Fail!!");
		return false;
	}

	if (searchedCount != 1)
	{
		DEBUG_BREAK();
		return false;
	}

	*ppOutAnimation = pTmpAnimation;
	return true;
}

void AnimationManager::CleanUp()
{
	HashTableIterator animTableEnd = animationTable_.end();
	for (HashTableIterator iter = animationTable_.begin(); iter != animTableEnd;)
	{
		Animation* pCurAnimation = static_cast<Animation*>(*iter);
		if (nullptr != pCurAnimation)
		{
			pCurAnimation->Release();
			pCurAnimation = nullptr;
		}
		iter = animationTable_.Delete(iter);
	}
}



