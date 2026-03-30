#include "stdafx.h"
#include "RunTimeMode.h"

RunTimeMode* RunTimeMode::GRunTimeMode = nullptr;

RunTimeMode::RunTimeMode()
	: curMode_(E_RUNTIME_MODE::EDITOR)
{
}

RunTimeMode::~RunTimeMode()
{
	CleanUp();
}

bool RunTimeMode::Create()
{
	RunTimeMode::GRunTimeMode = new RunTimeMode;
	return true;
}

void RunTimeMode::ShutDown()
{
	if (nullptr != RunTimeMode::GRunTimeMode)
	{
		delete RunTimeMode::GRunTimeMode;
		RunTimeMode::GRunTimeMode = nullptr;
	}
}

void RunTimeMode::ToggleRunTimeMode()
{
	switch (GRunTimeMode->curMode_)
	{	
	case E_RUNTIME_MODE::GAME:
		GRunTimeMode->curMode_ = E_RUNTIME_MODE::EDITOR;
		break;
	case E_RUNTIME_MODE::EDITOR:
		GRunTimeMode->curMode_ = E_RUNTIME_MODE::GAME;
		Editor::GetEditor()->SetGameModeFlag(false);
		break;
	default:
		break;
	}		
	
}

E_RUNTIME_MODE RunTimeMode::GetCurrentMode()
{
	return RunTimeMode::GRunTimeMode->curMode_;
}

void RunTimeMode::CleanUp()
{

}