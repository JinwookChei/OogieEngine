#pragma once

enum class E_RUNTIME_MODE
{
	GAME = 0,
	EDITOR
};

class RunTimeMode
{
	friend class Engine;

	RunTimeMode();
	~RunTimeMode();

	static RunTimeMode* GRunTimeMode;
	static bool Create();
	static void ShutDown();

public:
	static void ToggleRunTimeMode();
	static E_RUNTIME_MODE GetCurrentMode();

private:
	void CleanUp();

private:
	E_RUNTIME_MODE curMode_;
};
