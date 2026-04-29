#pragma once

class AnimationManager
{
	friend class Engine;
	AnimationManager();

	~AnimationManager();

public:
	static ENGINE_API  AnimationManager* Instance();

	void TestLoad();

	Animation* CreateAnimation(const std::string& fileName, bool bLoop, const char* animationKey, unsigned int keySize);

	ENGINE_API bool GetAnimation(Animation** ppOutAnimation, const char* animationKey, unsigned int keySize);

private:
	void CleanUp();

	HashTable animationTable_;
};