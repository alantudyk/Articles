#ifndef ___UTF_VL_H
#define ___UTF_VL_H

#include <stdint.h>

_Static_assert(__SIZEOF_POINTER__ == 8, "");

typedef struct junk_t { uint8_t *p; size_t s;             } junk_t;
typedef struct  str_t { uint8_t *p; size_t s, l; void *a; }  str_t;

typedef struct vl_iterator_t { uint8_t *p; size_t l; } vl_iterator_t;

 void vl_init(vl_iterator_t *iterator, const  str_t *s);
_Bool vl_next(vl_iterator_t *iterator,      int32_t *c);

_Bool vl_8_from_bytes(const uint8_t *p, size_t s, junk_t *_dest, size_t *tail);
_Bool   vl_from_bytes(const uint8_t *p, size_t s,  str_t *_dest, size_t *tail);

void vl_free(str_t *s);

size_t vl_char_size(const int32_t *c);

_Bool vl_char_at(const str_t *s, size_t i, int32_t *c);

_Bool vl_concat(const str_t *a, const str_t *b, str_t *_dest);

_Bool  vl_equal(const str_t *a, const str_t *b);
 int    vl_cmp (const str_t *a, const str_t *b);

_Bool vl_from_8(const junk_t *_8,  str_t *_s);
_Bool   vl_to_8(const  str_t *_s, junk_t *_8);

 void  vl_move(      str_t *_src, str_t *_dest);
_Bool vl_clone(const str_t *_src, str_t *_dest);

#endif
