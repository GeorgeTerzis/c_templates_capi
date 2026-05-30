#include <malloc.h>
#include <stdbool.h>

#define TI_TYPEDEF(name)                                                                 \
    typedef struct {                                                                     \
        int dummy;                                                                       \
    } name##_ti;

TI_TYPEDEF(first)
TI_TYPEDEF(second)

typedef struct {
    first_ti fst;
    second_ti snd;
} duo_te;

duo_te duo_make_te(first_ti fst, second_ti snd) {
    duo_te d;
    d.fst = fst;
    d.snd = snd;
    return d;
}
duo_te duo_swap_te(duo_te d) {
    duo_te swapped;
    swapped.fst = (first_ti)d.snd;
    swapped.snd = (second_ti)d.fst;
    return swapped;
}

duo_te* duo_alloc_te(first_ti fst, second_ti snd) {
    duo_te* d = malloc(sizeof(duo_te));
    d->fst = fst;
    d->snd = snd;
    return d;
}

bool duo_eq_ta(duo_te a, duo_te b) {
    return a.fst == b.fst && a.snd == b.snd;
}
