
#define a_ti int
#define b_ti float 

#define CONCATENATE1_(prefix, T0) prefix##_##T0 
#define CONCATENATE2_(prefix, T0, T1) prefix##_##T0##_##T1 
#define CONCATENATE3_(prefix, T0, T1, T2) prefix##_##T0##_##T1##_##T2 
#define CONCATENATE4_(prefix, T0, T1, T2, T3) prefix##_##T0##_##T1##_##T2##_##T3 
#define CONCATENATE5_(prefix, T0, T1, T2, T3, T4) prefix##_##T0##_##T1##_##T2##_##T3##_##T4 
#define CONCATENATE6_(prefix, T0, T1, T2, T3, T4, T5) prefix##_##T0##_##T1##_##T2##_##T3##_##T4##_##T5 
#define CONCATENATE7_(prefix, T0, T1, T2, T3, T4, T5, T6) prefix##_##T0##_##T1##_##T2##_##T3##_##T4##_##T5##_##T6 
#define CONCATENATE8_(prefix, T0, T1, T2, T3, T4, T5, T6, T7) prefix##_##T0##_##T1##_##T2##_##T3##_##T4##_##T5##_##T6##_##T7 
#define CONCATENATE9_(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8) prefix##_##T0##_##T1##_##T2##_##T3##_##T4##_##T5##_##T6##_##T7##_##T8 
#define CONCATENATE10_(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9) prefix##_##T0##_##T1##_##T2##_##T3##_##T4##_##T5##_##T6##_##T7##_##T8##_##T9 
#define CONCATENATE1(prefix, T0) CONCATENATE1_(prefix, T0) 
#define CONCATENATE2(prefix, T0, T1) CONCATENATE2_(prefix, T0, T1) 
#define CONCATENATE3(prefix, T0, T1, T2) CONCATENATE3_(prefix, T0, T1, T2) 
#define CONCATENATE4(prefix, T0, T1, T2, T3) CONCATENATE4_(prefix, T0, T1, T2, T3) 
#define CONCATENATE5(prefix, T0, T1, T2, T3, T4) CONCATENATE5_(prefix, T0, T1, T2, T3, T4) 
#define CONCATENATE6(prefix, T0, T1, T2, T3, T4, T5) CONCATENATE6_(prefix, T0, T1, T2, T3, T4, T5) 
#define CONCATENATE7(prefix, T0, T1, T2, T3, T4, T5, T6) CONCATENATE7_(prefix, T0, T1, T2, T3, T4, T5, T6) 
#define CONCATENATE8(prefix, T0, T1, T2, T3, T4, T5, T6, T7) CONCATENATE8_(prefix, T0, T1, T2, T3, T4, T5, T6, T7) 
#define CONCATENATE9(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8) CONCATENATE9_(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8) 
#define CONCATENATE10(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9) CONCATENATE10_(prefix, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)


#if !defined(a_ti)
  #error "Template Input: a_ti needs to be defined"
#endif

#if !defined(b_ti)
  #error "Template Input: b_ti needs to be defined"
#endif


#if !defined(default_TEMPLATE_AUXILIARY)
#define default_TEMPLATE_AUXILIARY 

#if defined(my_aux_NAME)
  #define my_aux my_aux_NAME
#endif

typedef struct {
  int b;
} my_aux;

int my_func_ta(int a);



#if defined(default_TEMPLATE_AUXILIARY_FUNC_IMPL)

int my_func_ta(int a){
  int c = 10; 
  return c;
}
#endif
#endif

#if defined(my_entity_te_NAME)
  #define my_entity_te my_entity_te_NAME
#else
  #define my_entity_te CONCATENATE2(my_entity_te,a_ti,b_ti)
#endif

#define func_b_te CONCATENATE1(func_b_te,b_ti)
#define my_func_te CONCATENATE1(my_func_te,b_ti)

typedef struct {
  a_ti my_field;
  int cint;
  a_ti* (^block_func)(b_ti);    
} my_entity_te;

int func_b_te(b_ti b);
int my_func_te(int a);

#if defined(default_TEMPLATE_ENTITY_FUNC_IMPL)

int func_b_te(b_ti b){
  return 0; 
}

int my_func_te(int a){
   int c = func_b_te((b_ti){10});
    return c;
}

#endif

#if defined(my_aux)
  #undef my_aux
#endif


#if defined(default_TEMPLATE_AUXILIARY_FUNC_IMPL)
  #undef default_TEMPLATE_AUXILIARY_FUNC_IMPL
#endif


#if defined(my_entity_te)
  #undef my_entity_te
#endif


#if defined(func_b_te)
  #undef func_b_te
#endif


#if defined(my_func_te)
  #undef my_func_te
#endif


#if defined(default_TEMPLATE_ENTITY_FUNC_IMPL)
  #undef default_TEMPLATE_ENTITY_FUNC_IMPL
#endif


#if defined(a_ti)
  #undef a_ti
#endif

#if defined(b_ti)
  #undef b_ti
#endif

