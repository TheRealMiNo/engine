#include "Window.h"


Window::Window()
{

}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
  case WM_CREATE:
	{
		// Event fired when the window is created
		// collected here..
		Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
		// .. and then stored for later lookup
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
		window->onCreate();
		break;
	}
  	case WM_DESTROY:
	{
		// Event fired when the window is destroyed
		Window* window =(Window*) GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}
    default:
        return ::DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0;
}


bool Window::init()
{
    WNDCLASSEX wc = {};  // Initialize the structure to zero.

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = &WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(nullptr);  // You should use GetModuleHandle to get the application instance.
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "MywindowClass";
    wc.lpszMenuName = nullptr;

    if (!::RegisterClassEx(&wc))
        return false;

    m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "MywindowClass", "DirectX Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, nullptr, nullptr, nullptr, this);

    if (!m_hwnd)
        return false;

    ::ShowWindow(m_hwnd, SW_SHOW);
    ::UpdateWindow(m_hwnd);

    // Set this flag to true to indicate that the window is initialized and running.
    m_is_run = true;
    return true;
}


bool Window::Broadcast()
{
    MSG msg;
    while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE)>0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    this->onUpdate();

    Sleep(0);
    return true;
}

bool Window::release()
{
    if (!::DestroyWindow(m_hwnd))
        return false;

    return true;
}

bool Window::isRun()
{
    return m_is_run;
}

void Window::onDestroy()
{
    m_is_run = false;
}

Window::~Window()
{

}