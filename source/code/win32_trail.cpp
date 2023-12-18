#include <windows.h>

LRESULT CALLBACK window_proc(
			     HWND window,
			     UINT message,
			     WPARAM wparam,
			     LPARAM lparam)
{
  LRESULT result = 0;

  switch (message)
    {
    case WM_SIZE:
      {
	//TODO: Do something
      } break;
    case WM_PAINT:
      {
	//TODO: Do something
      } break;
    case WM_CLOSE:
      {
	PostQuitMessage(0);
      } break;
    case WM_DESTROY:
      {
	PostQuitMessage(0);
      } break;
    default:
      {
	result = DefWindowProc(window, message, wparam, lparam);
      } break;
    }
  
  return result;
}

int WINAPI WinMain(
		   HINSTANCE instance,
		   HINSTANCE prev_instance,
		   LPSTR command,
		   int show)
{
  WNDCLASSA window_class;
  window_class.lpszClassName = "main_window_class";
  window_class.lpfnWndProc = window_proc;
  window_class.hInstance = instance;
  window_class.style = CS_HREDRAW | CS_VREDRAW;

  RegisterClassA(&window_class);

  HWND window_handle = CreateWindowA(
				     window_class.lpszClassName,
				     "Personal Trail",
				     WS_VISIBLE | WS_OVERLAPPEDWINDOW,
				     CW_USEDEFAULT,
				     CW_USEDEFAULT,
				     640, 480,
				     0, 0, instance, 0);

   if (window_handle)
    {
      MSG message;
      while (GetMessage(&message, 0, 0, 0))
	{
	  TranslateMessage(&message);
	  DispatchMessage(&message);
	}
    }
  
  return 0;
}
