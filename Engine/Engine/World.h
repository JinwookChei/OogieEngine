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
	void UpdateTick(unsigned long long deltaTime);

	void ChangeLevelInternal(Level* level);

	void CheckChangeLevel();

	Level* curLevel_;

	Level* nextLevel_;
};
