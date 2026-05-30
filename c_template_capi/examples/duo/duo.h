/* Input file: duo_template.h */
/* Prefix: default */

/* This file was automagicaly generated */
/* When defining the input types make sure that you use single word and no special characters types */
/* Which means you have to typedef types like pointers */


#if !defined(first_ti)
  #error "Template Input: first_ti needs to be defined"
#endif

#if !defined(second_ti)
  #error "Template Input: second_ti needs to be defined"
#endif

#define CONCAT_0_(prefix, T0) prefix##_##T0
#define CONCAT_0(prefix, T0) CONCAT_0_(prefix, T0)

#define CONCAT_1_(prefix, T0, T1) CONCAT_0_(prefix##_##T0, T1)
#define CONCAT_1(prefix, T0, T1) CONCAT_1_(prefix, T0, T1)

#define CONCAT_2_(prefix, T0, T1, T2) CONCAT_1_(prefix##_##T0, T1, T2)
#define CONCAT_2(prefix, T0, T1, T2) CONCAT_2_(prefix, T0, T1, T2)

#define CONCAT_3_(prefix, T0, T1, T2, T3) CONCAT_2_(prefix##_##T0, T1, T2, T3)
#define CONCAT_3(prefix, T0, T1, T2, T3) CONCAT_3_(prefix, T0, T1, T2, T3)

#define CONCAT_4_(prefix, T0, T1, T2, T3, T4) CONCAT_3_(prefix##_##T0, T1, T2, T3, T4)
#define CONCAT_4(prefix, T0, T1, T2, T3, T4) CONCAT_4_(prefix, T0, T1, T2, T3, T4)

#define CONCAT_5_(prefix, T0, T1, T2, T3, T4, T5) CONCAT_4_(prefix##_##T0, T1, T2, T3, T4, T5)
#define CONCAT_5(prefix, T0, T1, T2, T3, T4, T5) CONCAT_5_(prefix, T0, T1, T2, T3, T4, T5)

#define CONCAT_6_(prefix, T0, T1, T2, T3, T4, T5, T6) CONCAT_5_(prefix##_##T0, T1, T2, T3, T4, T5, T6)
#define CONCAT_6(prefix, T0, T1, T2, T3, T4, T5, T6) CONCAT_6_(prefix, T0, T1, T2, T3, T4, T5, T6)

#define CONCAT_7_(prefix, T0, T1, T2, T3, T4, T5, T6, T7) CONCAT_6_(prefix##_##T0, T1, T2, T3, T4, T5, T6, T7)
#define CONCAT_7(prefix, T0, T1, T2, T3, T4, T5, T6, T7) CONCAT_7_(prefix, T0, T1, T2, T3, T4, T5, T6, T7)

#define CONCAT_8_(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8) CONCAT_7_(prefix##_##T0, T1, T2, T3, T4, T5, T6, T7, T8)
#define CONCAT_8(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8) CONCAT_8_(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8)

#define CONCAT_9_(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9) CONCAT_8_(prefix##_##T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)
#define CONCAT_9(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9) CONCAT_9_(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)





#if defined(duo)
  #define duo_te duo
#else
  #define duo_te CONCAT_1(duo,first_ti,second_ti)
#endif

#define duo_make_te CONCAT_1(duo_make_te,first_ti,second_ti)
#define duo_swap_te CONCAT_1(duo_swap_te,first_ti,second_ti)
#define duo_alloc_te CONCAT_1(duo_alloc_te,first_ti,second_ti)

typedef struct {
    first_ti fst;
    second_ti snd;
} duo_te;

duo_te duo_make_te(first_ti fst, second_ti snd) ;
duo_te duo_swap_te(duo_te d) ;
duo_te* duo_alloc_te(first_ti fst, second_ti snd) ;

#if defined(default_TEMPLATE_ENTITY_FUNC_IMPL)

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

#endif


#if defined(duo_eq)
  #undef duo_eq
#endif


#if defined(duo_te)
  #undef duo_te
#endif


#if defined(duo_make_te)
  #undef duo_make_te
#endif


#if defined(duo_swap_te)
  #undef duo_swap_te
#endif


#if defined(duo_alloc_te)
  #undef duo_alloc_te
#endif


#if defined(default_TEMPLATE_ENTITY_FUNC_IMPL)
  #undef default_TEMPLATE_ENTITY_FUNC_IMPL
#endif



#if defined(first_ti)
  #undef first_ti
#endif

#if defined(second_ti)
  #undef second_ti
#endif

