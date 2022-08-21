#include "utf-vl.h"

void  vl_free( str_t *s) {
    free(s->p);
    *s = (str_t){};
}

void vl_rfree(rstr_t *r) {
    free(r->p);
    free(r->ap);
    *r = (rstr_t){};
}

_Bool  vl_equal(const  str_t *a, const  str_t *b) {
    return a == b || (a->s == b->s && !memcmp(a->p, b->p, a->s));
}

_Bool vl_requal(const rstr_t *a, const rstr_t *b) {
    return vl_equal((const str_t *)a, (const str_t *)b);
}

_Bool   vl_clone_to_rstr(const  str_t   *_vl, rstr_t  *_rstr) {
    if (_vl->s == 0) { *_rstr = (rstr_t){}; return 0; }
    str_t vl = { malloc(_vl->s) , _vl->s };
    if (vl.p == NULL) return 1;
    memcpy(vl.p, _vl->p, vl.s);
    return vl_move_to_rstr(&vl, _rstr);
}

_Bool vl_clone_from_rstr(const rstr_t *_rstr,  str_t    *_vl) {
    if (_rstr->s == 0) { *_vl = (str_t){}; return 0; }
    rstr_t r = {
        malloc(_rstr->s) , _rstr->s,
        malloc(_rstr->c * sizeof(void *)), _rstr->c
    };
    if (r.p == NULL || r.ap == NULL) return 1;
    memcpy(r.p , _rstr->p, r.s);
    memcpy(r.ap, _rstr->p, r.c * sizeof(void *));
    return vl_move_from_rstr(&r, _vl);
}
