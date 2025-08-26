#pragma once

class Level;
class Engine;

class World final
{
public:
	friend class Engine;

	World();

	virtual ~World();

	template<typename LevelType>
	LevelType* ChangeLevel()
	{
		LevelType* newLevel = new LevelType;

		ChangeLevelInternal(newLevel);

		return newLevel;
	}

private:
	ENGINE_API void ChangeLevelInternal(Level* level);

	void OnTick(double deltaTime);

	void OnRender();

	void CheckChangeLevel();

	void CleanUp();

	Level* curLevel_;

	Level* nextLevel_;
};
