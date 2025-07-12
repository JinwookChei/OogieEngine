#pragma once
class Window
{
public:
	Window() = delete;
	Window(const wchar_t* className, const wchar_t* windowText);
	~Window();

	bool Initialize();

	void Show() const;

private:
	LPCWSTR className_;

	LPCWSTR windowText_;

	HWND hWnd_;
};