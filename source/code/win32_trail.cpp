#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static bool running = false;

static void *bitmap_memory;
static BITMAPINFO bitmap_info;
static int bitmap_width, bitmap_height;
static int bytes_per_pixel = 4;

static void win32_resize_dib_section(
				     int width,
				     int height)
{
  if (bitmap_memory)
    {
      VirtualFree(bitmap_memory, 0, MEM_RELEASE);
    }
  
  bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
  bitmap_info.bmiHeader.biWidth = width;
  bitmap_info.bmiHeader.biHeight = height;
  bitmap_info.bmiHeader.biPlanes = 1;
  bitmap_info.bmiHeader.biBitCount = 32;
  bitmap_info.bmiHeader.biCompression = BI_RGB;

  bitmap_width = width;
  bitmap_height = height;
  
  bitmap_memory = VirtualAlloc(
			       0,
			       width * height * bytes_per_pixel,
			       MEM_COMMIT,
			       PAGE_READWRITE);
}

static void win32_update_window(
				HDC device_context,
				int window_width,
				int window_height)
{
  StretchDIBits(
		device_context,
		0, 0, window_width, window_height,
		0, 0, bitmap_width, bitmap_height,
		bitmap_memory,
		&bitmap_info,
		DIB_RGB_COLORS,
		SRCCOPY);
}

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
	RECT client_rect;
	GetClientRect(window, &client_rect);

	int width = client_rect.right - client_rect.left;
	int height = client_rect.bottom - client_rect.top;

	win32_resize_dib_section(width, height);
      } break;
    case WM_PAINT:
      {
	//TODO: Do something
      } break;
    case WM_CLOSE:
      {
	running = false;
      } break;
    case WM_DESTROY:
      {
	running = false;
      } break;
    default:
      {
	result = DefWindowProcA(window, message, wparam, lparam);
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
  WNDCLASSA window_class = {0};
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
      running = true;
      while (running)
	{
	  MSG message;
	  if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	    {
	      TranslateMessage(&message);
	      DispatchMessage(&message);
	    }

	  HDC device_context = GetWindowDC(window_handle);

	  RECT window_rect;
	  GetWindowRect(window_handle, &window_rect);

	  int window_width = window_rect.right - window_rect.left;
	  int window_height = window_rect.bottom - window_rect.top;
	  
	  win32_update_window(device_context, window_width, window_height);
	}
    }
  
  return 0;
}
