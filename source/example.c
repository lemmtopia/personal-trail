#include <stdio.h>
#include "lemonic.c"

int main(void)
{
    v2 v ={1.2, 0.9};
    printf("Vec: {%f, %f}\n", v.x, v.y);
    v = v2_mult_scalar(v, 3);
    printf("Vec after multiplication: {%f, %f}\n", v.x, v.y);
    v = v2_normalize(v);
    printf("Vec after normalization: {%f, %f}\n", v.x, v.y);

    return 0;
}
