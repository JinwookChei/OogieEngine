#pragma once

class InputLayout
{
public:
	InputLayout() = delete;

	InputLayout(IInputLayout* pInputLayout);

	virtual ~InputLayout();

	void Setting();

	void SetInputLayout(IInputLayout* pInputLayout);
private:
	void CleanUp();

	IInputLayout* pInputLayoutImpl_;
};
