#pragma once

class Level;
class Engine;

class World final
{

	friend class Engine;

	World();

	virtual ~World();

public:
	static World* Instance();

	template<typename LevelType>
	LevelType* ChangeLevel()
	{
		LevelType* newLevel = new LevelType;

		ChangeLevelInternal(newLevel);

		return newLevel;
	}

	Level* GetLevel() const;
	
private:
	ENGINE_API void ChangeLevelInternal(Level* level);

	void OnTick(double deltaTime);

	void OnRender();

	void OnBlit();

	void CheckChangeLevel();

	void CleanUp();

	Level* curLevel_;

	Level* nextLevel_;
};
