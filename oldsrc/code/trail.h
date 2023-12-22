#ifndef TRAIL_H
#define TRAIL_H

#include <stdint.h>

typedef uint8_t u8;
typedef uint32_t u32;

struct entity_t
{
     float x, y, width, height;
     float dx, dy;
     u32 color;
     bool flip;
     float speed;
     bool is_solid;
};

struct sprite_t
{
     int width, height;
     u32* pixels;
};

static void *bitmap_memory;
static int bitmap_width, bitmap_height;
static int bytes_per_pixel = 4;

static int time = 0;

entity_t make_entity(int x, int y, int width, int height, u32 color);

static void render_vaporwave();
static void draw_rectangle(int x, int y, int width, int height, u32 color);

static void load_ppm(sprite_t* sprite, const char* path);
static void draw_sprite(sprite_t sprite, int x, int y, bool flip);

static int win32_get_keyboard_keys();
//static void update_player(entity_t* player);

#endif
