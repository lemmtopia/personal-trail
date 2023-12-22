#include <Windows.h>
#include <d2d1.h>

static bool running = false;

ID2D1Factory* factory;
ID2D1HwndRenderTarget* render_target;

LRESULT CALLBACK win32_window_proc(
	HWND window_handle,
	UINT message,
	WPARAM wparam,
	LPARAM lparam)
{
	LRESULT result = 0;

	switch (message)
	{
		case WM_QUIT:
		{
			running = false;
		} break;
		case WM_DESTROY:
		{
			running = false;
		} break;
		default:
		{
			result = DefWindowProc(window_handle, message, wparam, lparam);
		} break;
	}

	return result;
}

int WINAPI WinMain(
	HINSTANCE instance,
	HINSTANCE prev_instance,
	LPSTR command,
	int command_show)
{
	/* Creating the window class */
	WNDCLASS window_class = { 0 };
	window_class.lpszClassName = L"win32_main_window_class";
	window_class.lpfnWndProc = win32_window_proc;
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.hInstance = instance;
	window_class.hCursor = LoadCursorW(NULL, IDC_ARROW);

	/* Create the window */
	if (SUCCEEDED(RegisterClassW(&window_class)))
	{
		HWND window_handle = CreateWindowW(
			window_class.lpszClassName,
			L"Personal Trail",
			WS_VISIBLE | WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			640, 480,
			0, 0, instance, 0);

		if (window_handle)
		{
			running = true;

			D2D1CreateFactory(
				D2D1_FACTORY_TYPE_SINGLE_THREADED,
				&factory);

			RECT rect;
			GetClientRect(window_handle, &rect);

			factory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(
					window_handle,
					D2D1::SizeU(
						rect.right - rect.left,
						rect.bottom - rect.top)),
				&render_target);

			ID2D1SolidColorBrush* brush = NULL;
			render_target->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&brush);

			while (running)
			{
				MSG message;
				if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}

				render_target->BeginDraw();

				render_target->FillRectangle(
					D2D1::RectF(
						rect.left,
						rect.top,
						rect.right,
						rect.bottom),
					brush);

				render_target->EndDraw();
			}
		}
	}

	return 0;
}