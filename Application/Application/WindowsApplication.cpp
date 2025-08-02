#include "stdafx.h"
#include "Window.h"
#include "WindowsApplication.h"

WindowsApplication* GApplication = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	//case WM_PAINT:
	//{
	//	PAINTSTRUCT ps;
	//	HDC hdc = BeginPaint(hwnd, &ps);

	//	// All painting occurs here, between BeginPaint and EndPaint.

	//	FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

	//	EndPaint(hwnd, &ps);
	//}
	//return 0;


	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


WindowsApplication::WindowsApplication(
	HINSTANCE hInstance,
	PWSTR pCmdLine,
	int nCmdShow,
	const wchar_t* iconPath)
	: hInstance_(hInstance),
	pCmdLine_(pCmdLine),
	nCmdShow_(nCmdShow),
	iconPath_(iconPath),
	iCon_(nullptr),
	mainWindow_(nullptr),
	refCount_(1),
	isApplicationQuit_(false)
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

bool __stdcall WindowsApplication::InitializeMainWindow(const wchar_t* className, const wchar_t* windowText)
{
	mainWindow_ = new Window(className, windowText);
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
	//wc.style = 0;                               // Window �׸��� Ư��.
	wc.lpfnWndProc = WindowProc;                // �޼��� �Լ� ����.
	//wc.cbClsExtra = 0;                          // �߰� �޸� ��� ����.
	//wc.cbWndExtra = 0;                          // �߰� �޸� ��� ����.
	wc.hInstance = hInstance_;                  // �ν��Ͻ� �ڵ�.
	wc.hIcon = iCon_;                               // ������ ����.
	//wc.hCursor = 0;                             // Ŀ�� ����.
	//wc.hbrBackground;                           // ��׶��� ����.
	//wc.lpszMenuName = NULL;                     // �޴� ��� ����.
	wc.lpszClassName = className;               // ������ Ŭ���� �̸�(�ĺ�)

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
