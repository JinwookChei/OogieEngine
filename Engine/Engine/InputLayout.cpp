#include "stdafx.h"
#include "InputLayout.h"

InputLayout::InputLayout(IInputLayout* pInputLayout)
	:pInputLayoutImpl_(pInputLayout)
{
}

InputLayout::~InputLayout()
{
	CleanUp();
}

void InputLayout::Setting()
{
	if (nullptr == pInputLayoutImpl_)
	{
		DEBUG_BREAK();
		return;
	}

	pInputLayoutImpl_->Setting();
}

void InputLayout::SetInputLayout(IInputLayout* pInputLayout)
{
	if (nullptr != pInputLayoutImpl_)
	{
		pInputLayoutImpl_->Release();
		pInputLayoutImpl_ = nullptr;
	}
	if (nullptr != pInputLayout)
	{
		pInputLayoutImpl_ = pInputLayout;
		pInputLayoutImpl_->AddRef();
	}
}

void InputLayout::CleanUp()
{
	if (nullptr != pInputLayoutImpl_)
	{
		pInputLayoutImpl_->Release();
		pInputLayoutImpl_ = nullptr;
	}
}