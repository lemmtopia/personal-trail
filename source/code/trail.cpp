#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include "trail.h"

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
	       u8 r, g, b; //WARN: Needs to be u8, not char. Investigate why later.
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
			 if (sprite.pixels[(i * sprite.width) + j] != 0xFF0000)
			 {
			      *pixel = sprite.pixels[(i * sprite.width) + j]; // set the value that pixel is pointing to
			 }
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
			 if (sprite.pixels[(i * sprite.width) + j] != 0xFF0000)
			 {
			      *pixel = sprite.pixels[(i * sprite.width) + j];
			 }
			 pixel++; 
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
	       *pixel = i * j + time;
	       pixel++;
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
		    *pixel = color;
		    pixel++;
	       }
	  }
	  row += line_offset;
     }
}

static void update_player(entity_t* player)
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
