#ifndef TRAIL_H
#define TRAIL_H

#include <stdint.h>

typedef uint8_t u8;
typedef uint32_t u32;

struct entity_t
{
  int x, y, width, height;
  float dx, dy;
  u32 color;
};

entity_t make_entity(int x, int y, int width, int height, u32 color);

static void render_vaporwave();
static void draw_rectangle(int x, int y, int width, int height, u32 color);

#endif
