#pragma once

class AnimationManager
{
	friend class Engine;
	AnimationManager();

	~AnimationManager();

public:
	static ENGINE_API  AnimationManager* Instance();

	void TestLoad();

	Animation* CreateAnimation(const std::string& fileName, bool bLoop, unsigned long long animationTag);

	ENGINE_API bool GetAnimation(Animation** ppOutAnimation, unsigned long long animationTag);

private:
	void CleanUp();

	HashTable animationTable_;
};