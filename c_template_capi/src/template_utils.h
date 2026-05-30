#pragma once

//#define TEMPLATE_TYPEDEF(N) typedef struct{int dummy;} T##N##$;

#define TI_TYPEDEF(name) typedef struct{int a;} name##_ti;
