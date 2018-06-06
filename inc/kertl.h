#ifndef KERTL_H
#define KERTL_H

#include <stdint.h>

#ifdef __cplusplus
#include <algorithm>
#endif

typedef void * Pvoid;


#if defined (KERTL_DLL)
#define KERTL_API __declspec(dllexport)
#else
#define KERTL_API __declspec(dllimport)
#endif

#define KERTL_ANYZISE  1





#endif // KERTL_H
