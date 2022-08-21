#ifndef ___UTF_VL_H
#define ___UTF_VL_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>

_Static_assert(__SIZEOF_POINTER__ == 8, "");

#define BITMASK_SHL(n, shl) (((1LU << (n)) - 1) << (shl))
#define BITMASK(n) BITMASK_SHL(n, 0)

#define fin(stop) for (ssize_t i = -1, _stop = stop; ++i < _stop;)
#define fix(start, stop, increment) for (ssize_t i = start, _stop = stop; i < _stop; i += increment)

typedef struct  str_t { uint8_t *p; size_t s; } str_t;
typedef struct rstr_t {
    uint8_t *p;
    size_t   s;
    void  **ap;
    size_t   c;
} rstr_t;

_Bool  vl_char_at(const  str_t *s, size_t i, uint32_t *c);
_Bool vl_rchar_at(const rstr_t *s, size_t i, uint32_t *c);

_Bool  vl_equal(const  str_t *a, const  str_t *b);
_Bool vl_requal(const rstr_t *a, const rstr_t *b);
  int   vl_cmp (const  str_t *a, const  str_t *b);
  int  vl_rcmp (const rstr_t *a, const rstr_t *b);



#endif
