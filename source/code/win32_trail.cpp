#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <xinput.h>

#include <stdio.h>
#include <stdlib.h>

#include "trail.h"

static bool running = false;

static void *bitmap_memory;
static BITMAPINFO bitmap_info;
static int bitmap_width, bitmap_height;
static int bytes_per_pixel = 4;

static int time = 0;

entity_t player;
entity_t wall;

typedef DWORD WINAPI win32_xinput_get_state(DWORD dwUserIndex, XINPUT_STATE* pState);
static win32_xinput_get_state* win32_XInputGetState;

bool xinput_loaded;

static void win32_load_xinput()
{
	HMODULE xinput_dll = LoadLibraryA("XInput1_4.dll");
	if (xinput_dll)
	{
		win32_XInputGetState = (win32_xinput_get_state*)GetProcAddress(xinput_dll, "XInputGetState");
		xinput_loaded = true;
	}
}

static void load_ppm(sprite_t* sprite, const char* path)
{
  FILE* file = fopen(path, "rb");
  if (file != NULL)
    {
      //TODO: Error handling

      fscanf(file, "P6\n %d %d\n", &sprite->width, &sprite->height);
      fseek(file, sizeof(char), SEEK_CUR);
      sprite->pixels = (u32*)malloc(sprite->width * sprite->height * 4);

      printf("%d %d\n", sprite->width, sprite->height);

      for (int i = 0; i < sprite->width * sprite->height; i++)
	{
	  char r, g, b;
	  fread(&r, 1, 1, file);
	  fread(&g, 1, 1, file);
	  fread(&b, 1, 1, file);

	  sprite->pixels[i] = (r << 16) | (g << 8) | b;
	}
	  
      fclose(file);
    }
  else
    {
      printf("could not open file");
    }
}

static void draw_sprite(sprite_t sprite, int x, int y, bool flip)
{
  u8* row = (u8*)bitmap_memory;
  int line_offset = bitmap_width * bytes_per_pixel;

  for (int i = 0; i < sprite.height; i++)
    {
      u32* pixel = (u32*)(row + (x * bytes_per_pixel) + (y * line_offset));

      if (!flip)
	{
	  for (int j = 0; j < sprite.width; j++)
	    {
	      if (i > 0 && i < bitmap_height && j > 0 && j < bitmap_width)
		{
		  *pixel = sprite.pixels[(i * sprite.width) + j]; // set the value that pixel is pointing to
		  pixel++; // offset pixel by one (move to the next memory address)
		}
	    }
	}
      else
	{
	   for (int j = sprite.width; j > 0; j--)
	    {
	      if (i > 0 && i < bitmap_height && j > 0 && j < bitmap_width)
		{
		  *pixel = sprite.pixels[(i * sprite.width) + j]; // set the value that pixel is pointing to
		  pixel++; // offset pixel by one (move to the next memory address)
		}
	    }
	}
      
      row += line_offset;
    }

}

entity_t make_entity(
		     int x,
		     int y,
		     int width,
		     int height,
                     u32 color)
{
  entity_t ent;

  ent.x = x;
  ent.y = y;
  ent.width = width;
  ent.height = height;
  ent.color = color;
    
  return ent;
}

static void render_vaporwave()
{
  u8* row = (u8*)bitmap_memory;
  int line_offset = bitmap_width * bytes_per_pixel;
  for (int i = 0; i < bitmap_height; i++)
    {
      u32* pixel = (u32*)row;
      for (int j = 0; j < bitmap_width; j++)
	{
	  *pixel = i * j + time; // set the value that pixel is pointing to
	  pixel++; // offset pixel by one (move to the next memory address)
	}
      row += line_offset;
    }
}

static void draw_rectangle(int x, int y, int width, int height, u32 color)
{
  u8* row = (u8*)bitmap_memory;
  int line_offset = bitmap_width * bytes_per_pixel;
  for (int i = 0; i < height; i++)
    {
      u32* pixel = (u32*)(row + (x * bytes_per_pixel) + (y * line_offset));
      for (int j = 0; j < width; j++)
	{
	  if (i > 0 && i < bitmap_height - height && j > 0 && j < bitmap_width - width)
	    {
	      *pixel = color; // set the value that pixel is pointing to
	      pixel++; // offset pixel by one (move to the next memory address)
	    }
	}
      row += line_offset;
    }
}

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
  bitmap_info.bmiHeader.biHeight = -height;
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
  window_class.hCursor = LoadCursorA(NULL, IDC_ARROW);

  RegisterClassA(&window_class);

  HWND window_handle = CreateWindowA(
				     window_class.lpszClassName,
				     "Personal Trail",
				     WS_VISIBLE | WS_MINIMIZEBOX | WS_OVERLAPPED | WS_BORDER | WS_SYSMENU,
				     CW_USEDEFAULT,
				     CW_USEDEFAULT,
				     640, 480,
				     0, 0, instance, 0);

  if (window_handle)
    {
      running = true;

      wall = make_entity(200, 80, 50, 80, 0x0000FF);

      sprite_t sprite, background;
      load_ppm(&sprite, "braid.ppm");
      load_ppm(&background, "bg.ppm");
      player = make_entity(80, 80, sprite.width, sprite.height, 0xFF00FF);

      player.speed = 0.69;
      
      win32_load_xinput();
  
      while (running)
	{
	  MSG message;
	  if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	    {
	      TranslateMessage(&message);
	      DispatchMessage(&message);
	    }

	  if (xinput_loaded)
	    {
	      for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
		  XINPUT_STATE controller_state;
		  if (win32_XInputGetState(i, &controller_state) == ERROR_SUCCESS)
		    {
		      XINPUT_GAMEPAD* gamepad = &controller_state.Gamepad;
		      
		      bool button_a = (gamepad->wButtons & XINPUT_GAMEPAD_A);
		      bool button_b = (gamepad->wButtons & XINPUT_GAMEPAD_B);
		      bool button_x = (gamepad->wButtons & XINPUT_GAMEPAD_X);
		      bool button_y = (gamepad->wButtons & XINPUT_GAMEPAD_Y);
		      
		      bool left = (gamepad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
		      bool right = (gamepad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);

		      SHORT stick = gamepad->sThumbLX;
		      
		      
		      if (stick < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || stick > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
			  player.dx += (float)stick / 4000000;

			  if (player.dx < -player.speed)
			    {
			      player.dx = -player.speed;
			    }
			  else if (player.dx > player.speed)
			    {
			      player.dx = player.speed;
			    }
			}
		      else
			{
			  player.dx *= 0.92;
			}

		      player.flip = false;
		      if (player.dx < 0)
			{
			  player.flip = true;
			}
		   		      
		      player.x += player.dx;
		      player.y += player.dy;
		    }
		}
	    }
	  
	  time += 10;
	  //render_vaporwave();
	  draw_sprite(background, 0, 0, false);
	  draw_rectangle(50, 40, 180, 80, 0xFFFFFF);
	  draw_rectangle((int)wall.x, (int)wall.y, (int)wall.width, (int)wall.height, (int)wall.color);
	  draw_sprite(sprite, (int)player.x, (int)player.y, player.flip);
	  
	  HDC device_context = GetWindowDC(window_handle);

	  RECT window_rect;
	  GetWindowRect(window_handle, &window_rect);

	  int window_width = window_rect.right - window_rect.left;
	  int window_height = window_rect.bottom - window_rect.top;
	  
	  win32_update_window(device_context, window_width, window_height);
	  ReleaseDC(window_handle, device_context);
	}
    }
  
  return 0;
}
