#include <malloc.h>

#define TI_TYPEDEF(name)                                                                 \
    typedef struct {                                                                     \
        int dummy;                                                                       \
    } name##_ti;

TI_TYPEDEF(a)
TI_TYPEDEF(b)

typedef struct {
    a_ti my_field;
    int cint;
    a_ti* (^block_func)(b_ti);
} my_entity_te;

typedef struct {
    int b;
} my_aux_ta;

int func_b_te(b_ti b) {
    return 0;
}

int my_func_ta(int a) {
    int c = 10;
    return c;
}

int my_func_te(int a) {
    int c = func_b_te((b_ti){10});
    return c;
}
