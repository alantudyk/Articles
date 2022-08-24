#ifndef ___UTF_VL_H
#define ___UTF_VL_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>

_Static_assert(__SIZEOF_POINTER__ == 8, "");

#define BITMASK_SHL(n, shl) (((1LU << (n)) - 1) << (shl))
#define BITMASK(n) BITMASK_SHL(n, 0)

#define fin(stop) for (ssize_t i = -1, _stop = stop; ++i < _stop;)
#define fiN(iter_name, stop) for (ssize_t iter_name = -1, _stop = stop; ++iter_name < _stop;)
#define fix(start, stop, increment) \
    for (ssize_t i = start, _stop = stop; i < _stop; i += increment)
#define fiX(iter_name, start, stop, increment) \
    for (ssize_t iter_name = start, _stop = stop; iter_name < _stop; iter_name += increment)

typedef struct  str_t { uint8_t *p; size_t s, l;             }  str_t;
typedef struct rstr_t { uint8_t *p; size_t s, l, z; void *a; } rstr_t;

_Bool vl_from_bytes(uint8_t *p, size_t s, str_t *_dest, size_t *tail);

void  vl_free( str_t *s);
void vl_rfree(rstr_t *r);

size_t vl_char_size(int32_t *c);

_Bool  vl_char_at(const  str_t *s, size_t i, int32_t *c);
_Bool vl_rchar_at(const rstr_t *r, size_t i, int32_t *c);

_Bool vl_concat(const str_t *a, const str_t *b, str_t *_dest);

_Bool  vl_equal(const  str_t *a, const  str_t *b);
_Bool vl_requal(const rstr_t *a, const rstr_t *b);
  int   vl_cmp (const  str_t *a, const  str_t *b);
  int  vl_rcmp (const rstr_t *a, const rstr_t *b);

_Bool vl_from_8(const str_t *_8, str_t *_s);
_Bool   vl_to_8(const str_t *_s, str_t *_8);

void  vl_move( str_t *_src,  str_t *_dest);
void vl_rmove(rstr_t *_src, rstr_t *_dest);

_Bool  vl_clone(const  str_t *_src,  str_t *_dest);
_Bool vl_rclone(const rstr_t *_src, rstr_t *_dest);

_Bool   vl_move_to_rstr( str_t *s, rstr_t *r);
 void vl_move_from_rstr(rstr_t *r,  str_t *s);

_Bool   vl_clone_to_rstr(const  str_t *s, rstr_t *r);
_Bool vl_clone_from_rstr(const rstr_t *r,  str_t *s);

#endif
