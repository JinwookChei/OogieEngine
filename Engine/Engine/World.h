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
	void UpdateTick(double deltaTime);

	ENGINE_API void ChangeLevelInternal(Level* level);

	void CheckChangeLevel();

	void CleanUp();

	Level* curLevel_;

	Level* nextLevel_;
};
