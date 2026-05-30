#include <stdio.h>

#define first_ti float
#define second_ti float
#include "duo.h"

int main() {
    duo_float_float* a = duo_alloc_te_float_float(10, 20);
    printf("%f, %f", a->fst, a->snd);
    return 0;
}
