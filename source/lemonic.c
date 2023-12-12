#include "lemonic.h"

#include <math.h>

v2 v2_mult_scalar(v2 vec, float scalar)
{
    v2 v = vec;
    v.x *= scalar;    
    v.y *= scalar;    
    return v;
}

v2 v2_normalize(v2 vec)
{
    v2 v = vec;
    float m = sqrt(v.x * v.x + v.y * v.y);
    v.x /= m;
    v.y /= m;
    return v;
}
