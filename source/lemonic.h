#ifndef LEMONIC_H
#define LEMONIC_H

#define LEMONIC_VERSION "0.69"

#ifndef LAPI
#define LAPI
#endif

// Basic Defines
#ifdef PI
#undef PI
#endif

#define PI 3.1415f

#define TRUE 1
#define FALSE 0

#ifdef ASSERT
#undef ASSERT
#endif

#define LEMONIC_ERROR -1
#define LEMONIC_SUCCESS 0

#define ASSERT(cond) if (!cond) return LEMONIC_ERROR

// Window defines
#define WIDTH 320
#define HEIGHT 180
#define SCALE 2

#define FPS 60
#define TARGET_FRAME_TIME 1000 / FPS

// Some colors
#define RED 0xDA122A
#define GREEN 0x12DA2A
#define BLUE 0x2A12DA
#define WHITE 0xFFFFFF
#define BLACK 0x000000

// Types
typedef struct
{
    float x, y;
} v2;

LAPI v2 v2_mult_scalar(v2 vec, float scalar);
LAPI v2 v2_normalize(v2 vec);

#endif
