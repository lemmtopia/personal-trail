#include <stdio.h>
#include "lemonic.c"

int main(void)
{
    v2 vec = {0.2, 1};
    printf("(%f, %f)\n", vec.x, vec.y);
    vec = v2_mult_scalar(vec, 2);
    printf("(%f, %f)\n", vec.x, vec.y);
    vec = v2_normalize(vec);
    printf("(%f, %f)\n", vec.x, vec.y);
    return 0;
}
