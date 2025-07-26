#include "stdafx.h"
#include "WindowsApplication.h"
#include "Window.h"

Window::Window(const wchar_t* className, const wchar_t* windowText)
	: className_(className),
	windowText_(windowText),
	hWnd_(nullptr)
{

}

Window::~Window()
{
	//delete HWnd
}

bool Window::Initialize()
{
	hWnd_ = CreateWindowEx(
		0,                              // Optional window styles.
		className_,                     // Window class
		windowText_,					// Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

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
