#include "utf-vl.h"

_Bool vl_from_bytes(uint8_t *p, size_t s, str_t *_dest, size_t *tail) {
    
    return 0;
}

void  vl_free( str_t *s) { free(s->p);             *s = ( str_t){}; }
void vl_rfree(rstr_t *r) { free(r->p); free(r->a); *r = (rstr_t){}; }

size_t vl_char_size(int32_t *c) {
    uint8_t *p = (uint8_t *)c;
    while (*p++ & 128);
    return (size_t)(p - (uint8_t *)c);
}

_Bool  vl_char_at(const  str_t *s, size_t _i, int32_t *_c) {
    if (_i >= s->l) return 1; *_c = 0;
    size_t i = 0; const uint8_t *p = s->p; uint8_t *c = (uint8_t *)_c;
    for (; i < _i; ++i) while (*p++ & 128);
    do *c++ = *p; while (*p++ & 128);
    return 0;
}

_Bool vl_rchar_at(const rstr_t *r, size_t _i, int32_t *c) {
    if (_i >= r->l) return 1;
    str_t s = {
        r->p + (r->s > (1L << 32) ? ((uint64_t *)r->a)[_i / 10] : ((uint32_t *)r->a)[_i / 10]),
        0, 10
    };
    vl_char_at(&s, _i % 10, c);
    return 0;
}

_Bool vl_concat(const str_t *a, const str_t *b, str_t *c) {
    if (a->s == 0) return vl_clone(b, c);
    if (b->s == 0) return vl_clone(a, c);
    *c = (str_t){ malloc(a->s + b->s), a->s + b->s, a->l + b->l };
    if (c->p == NULL) return 1;
    memcpy(c->p       , a->p, a->s);
    memcpy(c->p + a->s, b->p, b->s);
    return 0;
}

_Bool  vl_equal(const  str_t *a, const  str_t *b) {
    return a == b || (a->s == b->s && a->l == b->l && !memcmp(a->p, b->p, a->s));
}

_Bool vl_requal(const rstr_t *a, const rstr_t *b) {
    return vl_equal((const str_t *)a, (const str_t *)b);
}

int  vl_cmp(const  str_t *a, const  str_t *b) {
    if (a->l > b->l) { const void *t = a; a = b, b = t; }
    const uint8_t *pa = a->p, *pb = b->p;
    fiN(_, a->l) {
        int32_t ca = 0, cb = 0;
        uint8_t *cpa = (void *)&ca, *cpb = (void *)&cb;
        do *cpa++ = *pa; while (*pa++ & 128);
        do *cpb++ = *pb; while (*pb++ & 128);
        if (ca != cb) return ca - cb;
    }
    return -(b->l > a->l);
}

int vl_rcmp(const rstr_t *a, const rstr_t *b) {
    return vl_cmp((const str_t *)a, (const str_t *)b);
}

_Bool vl_from_8(const str_t *_8, str_t *_s) {
    
    return 0;
}

_Bool   vl_to_8(const str_t *_s, str_t *_8) {
    
    return 0;
}

void  vl_move( str_t *a,  str_t *b) { *b = *a; *a = ( str_t){}; }
void vl_rmove(rstr_t *a, rstr_t *b) { *b = *a; *a = (rstr_t){}; }

_Bool  vl_clone(const  str_t *a,  str_t *b) {
    if (a->s == 0) { *b = (str_t){}; return 0; }
    *b = *a, b->p = malloc(b->s);
    if (b->p == NULL) return 1;
    memcpy(b->p, a->p, b->s);
    return 0;
}

_Bool vl_rclone(const rstr_t *a, rstr_t *b) {
    if (a->s == 0) { *b = (rstr_t){}; return 0; }
    *b = *a, b->p = malloc(b->s), b->a = malloc(b->z);
    if (b->p == NULL || b->a == NULL) return 1;
    memcpy(b->p, a->p, b->s);
    memcpy(b->a, a->a, b->z);
    return 0;
}

_Bool   vl_move_to_rstr( str_t *s, rstr_t *r) {
    if (s->s == 0) { *r = (rstr_t){}; return 0; }
    _Bool Z = s->s > (1L << 32);
    if ((r->a = malloc(r->z = (s->l / 10 + 1) * 4 * (1 + Z))) == NULL) return 1;
    *(str_t *)r = *s; *s = (str_t){};
    const uint8_t *p = r->p, *const P = p + r->s;
    fiN(j, r->l / 10 + 1) {
        if (Z) ((uint64_t *)r->a)[j] = p - r->p; else ((uint32_t *)r->a)[j] = p - r->p;
        fiN(_, 10) { while (*p++ & 128); if (p == P) break; }
    }
    return 0;
}

 void vl_move_from_rstr(rstr_t *r,  str_t *s) {
     *s = *(str_t *)r; r->p = NULL; vl_rfree(r);
}

_Bool   vl_clone_to_rstr(const  str_t *s, rstr_t *r) {
    if (s->s == 0) { *r = (rstr_t){}; return 0; }
    str_t c; if (vl_clone(s, &c)) return 1;
    if (vl_move_to_rstr(&c, r)) { vl_free(&c); return 1; }
    return 0;
}

_Bool vl_clone_from_rstr(const rstr_t *r,  str_t *s) {
    if (r->s == 0) { *s = (str_t){}; return 0; }
    rstr_t c = {}; if (vl_clone((const str_t *)r, (str_t *)&c)) return 1;
    vl_move_from_rstr(&c, s); return 0;
}
