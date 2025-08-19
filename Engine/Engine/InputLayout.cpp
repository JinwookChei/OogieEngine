#include "stdafx.h"
#include "InputLayout.h"

InputLayout::InputLayout()
	:pInputLayout_(nullptr)
{
}

InputLayout::~InputLayout()
{
	if (nullptr != pInputLayout_)
	{
		pInputLayout_->Release();
		pInputLayout_ = nullptr;
	}
}
