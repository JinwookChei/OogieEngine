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
	Animation* Anim_Capoeira = CreateAnimation("../Resource/Fbx/Mixamo/Capoeira.FBX", true, 0);
	Animation* Anim_Werewolf_idle1 = CreateAnimation("../Resource/Fbx/WereWolf/Animation/Anim_Werewolf_idle1.FBX", true, 1);
	Animation* Anim_Werewolf_idle2 = CreateAnimation("../Resource/Fbx/WereWolf/Animation/Anim_Werewolf_idle2.FBX", true, 2);
	Animation* Anim_Werewolf_walk = CreateAnimation("../Resource/Fbx/WereWolf/Animation/Anim_Werewolf_walk.FBX", true, 3);
	Animation* Anim_Werewolf_attack1 = CreateAnimation("../Resource/Fbx/WereWolf/Animation/Anim_Werewolf_attack1.FBX", false, 4);
	Animation* Anim_Werewolf_death1 = CreateAnimation("../Resource/Fbx/WereWolf/Animation/Anim_Werewolf_death1.FBX", false, 5);
	Animation* Anim_Werewolf_gethit1 = CreateAnimation("../Resource/Fbx/WereWolf/Animation/Anim_Werewolf_gethit1.FBX", false, 6);
}

Animation* AnimationManager::CreateAnimation(const std::string& fileName, bool bLoop, unsigned long long animationTag)
{
	Animation* pNewAnimation = new Animation;
	if (false == FBXManager::LoadAnimation(pNewAnimation, fileName))
	{
		DEBUG_BREAK();
		delete pNewAnimation;
		
		return nullptr;
	}
	
	pNewAnimation->animationClip_.bLoop = bLoop;
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



