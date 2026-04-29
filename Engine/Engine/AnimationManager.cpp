#include "stdafx.h"
#include "AnimationManager.h"


AnimationManager::AnimationManager()
	: animationTable_(32, 32)
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
	Animation* pAnim_Capoeira = CreateAnimation("../Resource/Fbx/Mixamo/Capoeira.FBX", true, "AM_Capoeira", 11);

	Animation* pAnim_Werewolf_Idle1 = CreateAnimation("../Resource/Fbx/WereWolf/Animation/Anim_Werewolf_idle1.FBX", true, "AM_Werewolf_Idle1", 17);
	Animation* pAnim_Werewolf_Idle2 = CreateAnimation("../Resource/Fbx/WereWolf/Animation/Anim_Werewolf_idle2.FBX", true, "AM_Werewolf_Idle2", 17);
	Animation* pAnim_Werewolf_Walk = CreateAnimation("../Resource/Fbx/WereWolf/Animation/Anim_Werewolf_walk.FBX", true, "AM_Werewolf_Walk", 16);
	Animation* pAnim_Werewolf_Attack1 = CreateAnimation("../Resource/Fbx/WereWolf/Animation/Anim_Werewolf_attack1.FBX", false, "AM_Werewolf_Attack", 18);
	Animation* pAnim_Werewolf_Death1 = CreateAnimation("../Resource/Fbx/WereWolf/Animation/Anim_Werewolf_death1.FBX", false, "AM_Werewolf_Death", 17);
	Animation* pAnim_Werewolf_Hit = CreateAnimation("../Resource/Fbx/WereWolf/Animation/Anim_Werewolf_gethit1.FBX", false, "AM_Werewolf_Hit", 15);
}

Animation* AnimationManager::CreateAnimation(const std::string& fileName, bool bLoop, const char* animationKey, unsigned int keySize)
{
	Animation* pNewAnimation = new Animation;
	if (false == FBXManager::LoadAnimation(pNewAnimation, fileName))
	{
		DEBUG_BREAK();
		delete pNewAnimation;
		return nullptr;
	}

	pNewAnimation->animationClip_.bLoop = bLoop;
	animationTable_.Insert(pNewAnimation, animationKey, keySize);
	return pNewAnimation;
}

bool AnimationManager::GetAnimation(Animation** ppOutAnimation, const char* animationKey, unsigned int keySize)
{
	unsigned int searchedCount = 0;
	Animation* pTmpAnimation = nullptr;
	if (false == animationTable_.Search((void**)&pTmpAnimation, &searchedCount, 8, animationKey, keySize))
	{
		DEBUG_BREAK();
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



