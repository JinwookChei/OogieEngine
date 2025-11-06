#include "stdafx.h"
#include "WindowsApplication.h"
#include "Window.h"

Window::Window(const wchar_t* className, const wchar_t* windowText, const Float2& windowSize)
	: className_(className),
	windowText_(windowText),
	hWnd_(nullptr),
	windowSize_(windowSize)
{

}

Window::~Window()
{
	//delete HWnd
}

bool Window::Initialize()
{
	DWORD dwStyle = WS_POPUP;
	RECT rc = { 0, 0, windowSize_.X, windowSize_ .Y};
	AdjustWindowRect(&rc, dwStyle, FALSE);

	hWnd_ = CreateWindowEx(
		0,                              // Optional window styles.
		className_,                     // Window class
		windowText_,					// Window text
		dwStyle,						// Window style
		CW_USEDEFAULT, CW_USEDEFAULT,	// Size and position
		rc.right - rc.left,             // 전체 윈도우 폭 (조정된 값)
		rc.bottom - rc.top,
		NULL,       // Parent window    
		NULL,       // Menu
		GApplication->HandleInstance(),  // Instance handle
		NULL        // Additional application data
	);

	if (hWnd_ == NULL)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

HWND Window::Handle() const
{
	return hWnd_;
}

void Window::Show() const
{
	ShowWindow(hWnd_, GApplication->CmdShowParam());
}
