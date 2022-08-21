#include "utf-vl.h"

void  vl_free( str_t *s) {
    free(s->p);
    *s = (str_t){};
}

void vl_rfree(rstr_t *r) {
    free(r->p);
    free(r->a);
    *r = (rstr_t){};
}

_Bool  vl_equal(const  str_t *a, const  str_t *b) {
    return a == b || (a->s == b->s && !memcmp(a->p, b->p, a->s));
}

_Bool vl_requal(const rstr_t *a, const rstr_t *b) {
    return vl_equal((const str_t *)a, (const str_t *)b);
}

void  vl_move( str_t *a,  str_t *b) { *b = *a; *a = ( str_t){}; }
void vl_rmove(rstr_t *a, rstr_t *b) { *b = *a; *a = (rstr_t){}; }

_Bool  vl_clone(const  str_t *a,  str_t *b) {
    if (a->s == 0) { *b = (str_t){}; return 0; }
    *b = (str_t){ malloc(a->s) , a->s };
    if (b->p == NULL) return 1;
    memcpy(b->p, a->p, b->s);
    return 0;
}

_Bool vl_rclone(const rstr_t *a, rstr_t *b) {
    if (a->s == 0) { *b = (rstr_t){}; return 0; }
    *b = (rstr_t){
        malloc(a->s) , a->s,
        malloc(a->c * sizeof(uint8_t *)), a->c
    };
    if (b->p == NULL || b->a == NULL) return 1;
    memcpy(b->p, a->p, b->s);
    memcpy(b->a, a->a, b->c * sizeof(uint8_t *));
    return 0;
}

_Bool   vl_move_to_rstr( str_t   *_vl, rstr_t *_rstr) {
    
    return 0;
}

 void vl_move_from_rstr(rstr_t *_rstr,  str_t   *_vl) {
     *_vl = *(str_t *)_rstr; _rstr->p = NULL; vl_rfree(_rstr);
}

_Bool   vl_clone_to_rstr(const  str_t   *_vl, rstr_t  *_rstr) {
    if (_vl->s == 0) { *_rstr = (rstr_t){}; return 0; }
    str_t vl; return vl_clone(_vl, &vl) || vl_move_to_rstr(&vl, _rstr);
}

_Bool vl_clone_from_rstr(const rstr_t *_rstr,  str_t    *_vl) {
    if (_rstr->s == 0) { *_vl = (str_t){}; return 0; }
    rstr_t r; if (vl_rclone(_rstr, &r)) return 1;
    vl_move_from_rstr(&r, _vl); return 0;
}
