#pragma once

class EditorWidget
	: public IUnknown
{	
	/*enum class eState
	{
		Paused,
		Active,
		Disabled,
		Destroyed
	};*/
public:
	EditorWidget();
	virtual ~EditorWidget();

	//virtual void Init();
	virtual void Update() = 0;
	virtual void Render() = 0;
	/*virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnDestroy();*/

private:
	virtual void CleanUp();

	ULONG refCount_;
};