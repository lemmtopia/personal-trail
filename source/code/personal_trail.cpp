#include <windows.h>

#include "trail.cpp"

static int win32_get_keyboard_keys()
{
     int key_comb = 0;
     
     if (GetKeyState(VK_RIGHT) & 0x8000)
     {
	  key_comb++;
     }
     if (GetKeyState(VK_LEFT) & 0x8000)
     {
	  key_comb += 2;
     }
     if (GetKeyState(VK_SPACE) & 0x8000)
     {
	  key_comb += 4;
     }
     
     return key_comb;
}


extern "C" _declspec(dllexport) void update_player(entity_t* player)
{
     int keys = win32_get_keyboard_keys();
     if (keys == 1)
     {
	  player->dx += 0.1;
     }
     else if (keys == 2)
     {
	  player->dx -= 0.1;
     }
     else
     {
	  player->dx *= 0.92;
     }
     
     if (player->dx < -player->speed)
     {
	  player->dx = -player->speed;
     }
     else if (player->dx > player->speed)
     {
	  player->dx = player->speed;
     }
     
     if (player->x < 0)
     {
	  player->x = 0;
     }
     else if (player->x > 640 - player->width)
     {
	  player->x = 640 - player->width;
     }
     
     player->flip = false;
     if (player->dx < 0)
     {
	  player->flip = true;
     }
     
     player->x += player->dx;
     player->y += player->dy;
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved )  // reserved
{
     return true;
}
