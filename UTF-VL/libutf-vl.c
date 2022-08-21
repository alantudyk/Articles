#include "utf-vl.h"

_Bool  vl_equal(const  str_t *a, const  str_t *b) {
    return a == b || (a->s == b->s && !memcmp(a->p, b->p, a->s));
}

_Bool vl_requal(const rstr_t *a, const rstr_t *b) {
    return a == b || (a->s == b->s && !memcmp(a->p, b->p, a->s));
}
