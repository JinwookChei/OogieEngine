#include "stdafx.h"
#include "Window.h"

#include "WindowsApplication.h"

WindowsApplication* GApplication = nullptr;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (Editor::GetEditor()->WndProcHandler(hwnd, uMsg, wParam, lParam))
	{
		return true;	
	}
	
	/*if (ImGuiSystem::GetImGuiManager()->WndProcHandler(hwnd, uMsg, wParam, lParam))
	{
		return true;
	}*/


	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MOUSEMOVE:
		float posX = GET_X_LPARAM(lParam);
		float posY = GET_Y_LPARAM(lParam);

		Float2 mousePos{ posX , posY};
		GApplication->UpdateMousePosition(mousePos);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


WindowsApplication::WindowsApplication
(
	HINSTANCE hInstance,
	PWSTR pCmdLine,
	int nCmdShow,
	const wchar_t* iconPath
)
	: hInstance_(hInstance),
	pCmdLine_(pCmdLine),
	nCmdShow_(nCmdShow),
	iconPath_(iconPath),
	iCon_(nullptr),
	mainWindow_(nullptr),
	refCount_(1),
	isApplicationQuit_(false),
	mousePos_({ 0.0f, 0.0f })
{
	GApplication = this;
}

WindowsApplication::~WindowsApplication()
{
	if (nullptr != mainWindow_)
	{
		delete mainWindow_;
		mainWindow_ = nullptr;
	}
}

HRESULT __stdcall WindowsApplication::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall WindowsApplication::AddRef(void)
{
	return ++refCount_;
}

ULONG __stdcall WindowsApplication::Release(void)
{
	--refCount_;
	ULONG tempRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tempRefCount;
}

bool __stdcall WindowsApplication::InitializeMainWindow(const wchar_t* className, const wchar_t* windowText, const Float2& windowSize)
{
	mainWindow_ = new Window(className, windowText, windowSize);
	if (nullptr == mainWindow_)
	{
		DEBUG_BREAK();
		return false;
	}

	if (nullptr == iconPath_)
	{
		DEBUG_BREAK();
		return false;
	}

	iCon_ = (HICON)LoadImage(NULL, iconPath_, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
	if (nullptr == iCon_)
	{
		DEBUG_BREAK();
		return false;
	}

	WNDCLASS wc = {};
	//wc.style = 0;                               // Window 그리기 특성.
	wc.lpfnWndProc = WindowProc;                // 메세지 함수 지정.
	//wc.cbClsExtra = 0;                          // 추가 메모리 사용 안함.
	//wc.cbWndExtra = 0;                          // 추가 메모리 사용 안함.
	wc.hInstance = hInstance_;                  // 인스턴스 핸들.
	wc.hIcon = iCon_;                               // 아이콘 설정.
	//wc.hCursor = 0;                             // 커서 설정.
	//wc.hbrBackground;                           // 백그라운드 설정.
	//wc.lpszMenuName = NULL;                     // 메뉴 사용 설정.
	wc.lpszClassName = className;               // 윈도우 클래스 이름(식별)

	RegisterClass(&wc);


	if (false == mainWindow_->Initialize())
	{
		return false;
	}

	mainWindow_->Show();

	return true;
}

void __stdcall WindowsApplication::WinPumpMessage()
{
	MSG message = {};
	while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_DESTROY) {
			if (isApplicationQuit_) {
				break;
			}
		}
		if (message.message == WM_QUIT) {
			isApplicationQuit_ = true;
		}
	}
}

bool __stdcall WindowsApplication::ApplicationQuit()
{
	return isApplicationQuit_;
}

void __stdcall WindowsApplication::SetShowCursor(bool show)
{
	if (show) {
		ShowCursor(TRUE);
	}
	else {
		ShowCursor(FALSE);
	}
}

void* __stdcall WindowsApplication::GetMainWindowHandle()
{
	return mainWindow_->Handle();
}

const Float2& __stdcall WindowsApplication::GetMousePosition() const
{
	return mousePos_;
}

void __stdcall WindowsApplication::Quit()
{
	if (nullptr == mainWindow_->Handle())
	{
		DEBUG_BREAK();
	}
	SendMessage(mainWindow_->Handle(), WM_CLOSE, 0, 0);
}

void WindowsApplication::UpdateMousePosition(const Float2& mousePos)
{
	mousePos_ = mousePos;
}

const HINSTANCE WindowsApplication::HandleInstance() const
{
	return hInstance_;
}

Window* WindowsApplication::MainWindow() const
{
	return mainWindow_;
}

const PWSTR WindowsApplication::CmdLine() const
{
	return pCmdLine_;
}

const int WindowsApplication::CmdShowParam() const
{
	return nCmdShow_;
}



