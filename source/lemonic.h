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

#define ASSERT(cond) return cond - 1 // ERROR or SUCCESS
                                
// Some colors
#define RED 0xDA122A
#define WHITE 0xFFFFFF
#define BLACK 0x000000

    // Types
typedef struct
{
    float x, y;
} v2;

typedef struct
{
    void* data;
    int w, h;
} tex; // RRGGBB

#endif
