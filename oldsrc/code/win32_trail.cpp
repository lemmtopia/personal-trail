#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <xinput.h>

#include <stdio.h>
#include <stdlib.h>

#include "trail.h"
#include "trail.cpp"

static bool running = false;

static BITMAPINFO bitmap_info;

entity_t entities[10];
int entity_num = 0;

typedef DWORD WINAPI win32_xinput_get_state(DWORD dwUserIndex, XINPUT_STATE* pState);
static win32_xinput_get_state *win32_XInputGetState;

typedef void WINAPI update_player_t(entity_t* player); // Function pointer to the DLL function.
static update_player_t *update_player;

bool xinput_loaded;
static void win32_load_xinput()
{
  // Use dynamic library, because I want people that don't have XInput to be able to play.
  // If we use DLLs, we can easily not execute the code if the user doesn't have it.
     HMODULE xinput_dll = LoadLibraryA("XInput1_4.dll"); 
     if (xinput_dll)
     {
	  win32_XInputGetState = (win32_xinput_get_state*)GetProcAddress(xinput_dll, "XInputGetState");
	  xinput_loaded = true;
     }
}

static void win32_load_game()
{
     HMODULE game_dll = LoadLibraryA("game.dll");
     if (game_dll)
     {
	  update_player = (update_player_t*)GetProcAddress(game_dll, "update_player");
	  running = true;
     }
}

static void win32_resize_dib_section(
     int width,
     int height)
{
  if (bitmap_memory)
  {
       VirtualFree(bitmap_memory, 0, MEM_RELEASE); // Free the memory to allocate it again later.
  }
  
  bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
  bitmap_info.bmiHeader.biWidth = width;
  bitmap_info.bmiHeader.biHeight = -height;
  bitmap_info.bmiHeader.biPlanes = 1;
  bitmap_info.bmiHeader.biBitCount = 32;
  bitmap_info.bmiHeader.biCompression = BI_RGB;
  
  bitmap_width = width;
  bitmap_height = height; // Easy access variables.
  
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
	  SRCCOPY); // This copies the bitmap to the given device context handle.
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
	  running = false;
	  win32_load_game();
	  entities[1] = make_entity(20, 340, 360, 80, 0x0000FF);

	  entities[1].is_solid = true;
	  
	  sprite_t player_sprite, background;
	  load_ppm(&player_sprite, "assets/player.ppm");
	  load_ppm(&background, "assets/bg.ppm");
	  entities[0] = make_entity(100, 120, player_sprite.width, player_sprite.height, 0xFF00FF);
	  
	  entities[0].speed = 0.69;
	  entities[0].dx = 1;
	  
	  win32_load_xinput();

	  entity_num = 2;
	  
	  while (running)
	  {
	       MSG message;
	       if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	       {
		    TranslateMessage(&message);
		    DispatchMessage(&message);
	       }
	       
	       // XInput stuff
	       if (xinput_loaded && time > 5)
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
			      
			      //bool left = (gamepad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
			      //bool right = (gamepad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
			      
			      SHORT stick_horizontal = gamepad->sThumbLX;
			      SHORT stick_vertical = gamepad->sThumbLY;
			      
			      if (stick_horizontal < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || stick_horizontal > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			      {
				   entities[0].dx += (float)stick_horizontal / 400000;
			      }		     
			 }
		    }
	       }
	       
	       // General player stuff
	       update_player(&entities[0]); // first entity will always be the player
	       
	       // Rendering stuff
	       time++;
	       //render_vaporwave();
	       draw_sprite(background, 0, 0, false);
	       draw_rectangle(50, 40, 180, 80, 0xFFFFFF);

	       for (int i = 1; i < entity_num; i++)
	       {
		    draw_rectangle((int)entities[i].x, (int)entities[i].y, (int)entities[i].width, (int)entities[i].height, (int)entities[i].color);	    
	       }

	       draw_sprite(player_sprite, (int)entities[0].x, (int)entities[0].y, entities[0].flip);
	       
	       HDC device_context = GetWindowDC(window_handle);
	       
	       RECT window_rect;
	       GetWindowRect(window_handle, &window_rect);
	       
	       int window_width = window_rect.right - window_rect.left;
	       int window_height = window_rect.bottom - window_rect.top;
	       
	       win32_update_window(device_context, window_width, window_height); // Copy the bitmap to the window
	       ReleaseDC(window_handle, device_context);
	  }
     }
     
     return 0;
}
