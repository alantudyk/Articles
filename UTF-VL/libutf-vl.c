#include "utf-vl.h"

void  vl_init(vl_iterator_t *i, const  str_t *s) {
    *i = (vl_iterator_t){ s->p, s->l };
}

void vl_rinit(vl_iterator_t *i, const rstr_t *r) {
    vl_init(i, (const str_t *)r);
}

_Bool vl_next(vl_iterator_t *i, int32_t *_c) {
    if (i->l == 0) return 1; i->l--, *_c = 0;
    uint8_t *c = (uint8_t *)_c;
    do *c++ = *(i->p); while (*(i->p)++ & 128);
    return 0;
}

static const  uint8_t junk_S[256] = {
    
    // x = [1] * 128 + [0] * 64 + [2] * 32 + [3] * 16 + [4] * 8 + [0] * 8
    // 16.times do puts '    ' + x[_1 * 16, 16] * ', ' + ?, end
    
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0,
    
};

static const uint32_t junk_L[5] = { 0,         0,     1 << 7,    1 << 11,    1 << 16  },
                      junk_M[5] = { 0, BITMASK(7), BITMASK(5), BITMASK(4), BITMASK(3) };

_Bool vl_8_from_bytes(const uint8_t *p, size_t s, junk_t *_dest, size_t *tail) {
    const uint8_t *t = p + s, *P = t - 3;
    
    return 0;
}

_Bool   vl_from_bytes(const uint8_t *p, size_t s,  str_t *_dest, size_t *tail) {
    const uint8_t *t = p + s, *P = t;
    fin(3) { if (P == p) return 1; if (P[-1] & 128) --P; else break; }
    if ((*tail = t - P) == 3) return 1; P -= 2;
    *_dest = (str_t){ (uint8_t *)p, s - *tail, 0 };
    while (p < P) {
        uint32_t c = 0; _dest->l++;
        fin(3) {
            c |= (*p & BITMASK(7)) << (7 * i);
            if ((*p++ & 128) == 0) break;
        }
        if (p[-1] & 128) return 1;
        if (c > 1114111 - ((1 << 14) + 128)) return 1;
    }
    switch ((P + 2) - p) {
        case 1: _dest->l++; break;
        case 2: _dest->l += (*p & 128) ? 1 : 2;
    }
    return 0;
}

void  vl_free( str_t *s) { free(s->p);             *s = ( str_t){}; }
void vl_rfree(rstr_t *r) { free(r->p); free(r->a); *r = (rstr_t){}; }

size_t vl_char_size(const int32_t *c) {
    const uint8_t *p = (const uint8_t *)c;
    while (*p++ & 128);
    return (size_t)(p - (const uint8_t *)c);
}

_Bool  vl_char_at(const  str_t *s, size_t _i, int32_t *_c) {
    if (_i >= s->l) return 1; *_c = 0;
    const uint8_t *p = s->p; uint8_t *c = (uint8_t *)_c;
    fin(_i) while (*p++ & 128);
    do *c++ = *p; while (*p++ & 128);
    return 0;
}

_Bool vl_rchar_at(const rstr_t *r, size_t _i, int32_t *c) {
    if (_i >= r->l) return 1;
    const str_t s = {
        r->p + (r->s > (1L << 32) ? ((uint64_t *)r->a)[_i / 16] : ((uint32_t *)r->a)[_i / 16]),
        0, 16
    };
    vl_char_at(&s, _i % 16, c);
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

_Bool vl_from_8(const junk_t *_8,  str_t *_s) {
    if (_8->s == 0) { *_s = ( str_t){}; return 0; }
    uint8_t *o = _s->p = malloc(_8->s); if (o == NULL) return 1;
    const uint8_t *p = _8->p, *const P = p + _8->s; _s->l = 0;
    while (p < P) {
        uint32_t char_size = junk_S[*p], c = *p++ & junk_M[char_size]; _s->l++;
        fix (1, char_size, 1) c = (c << 6) | (*p++ & BITMASK(6));
        if (c < 128) *o++ = c; else if ((c -= 128) < (1 << 14)) {
            *o++ = 128 | (c & BITMASK(7)), *o++ = c >> 7;
        } else {
            *o++ = 128 | ((c -= 1 << 14) & BITMASK(7)),
            *o++ = 128 | ((c >>       7) & BITMASK(7)), *o++ = c >> 14;
        }
    }
    _s->p = realloc(_s->p, _s->s = o - _s->p);
    return 0;
}

_Bool   vl_to_8(const  str_t *_s, junk_t *_8) {
    if (_s->s == 0) { *_8 = (junk_t){}; return 0; }
    uint8_t *o = _8->p = malloc(_s->s * 2); if (o == NULL) return 1;
    const uint8_t *p = _s->p, *_p, *const P = p + _s->s;
    while (p < P) {
        uint32_t c = 0, char_size; _p = p;
        fin(3) {
            c |= (*p & BITMASK(7)) << (7 * i);
            if ((*p++ & 128) == 0) break;
        }
        char_size = p - _p;
        if (char_size > 1) c += (char_size < 3) ? 128 : (1 << 14) + 128;
        if (c < 128) *o++ = c; else if (c < (1 << 11)) {
            *o++ = 192 | (c >>  6), *o++ = 128 | ( c        & BITMASK(6));
        } else if (c < (1 << 16)) {
            *o++ = 224 | (c >> 12), *o++ = 128 | ((c >> 6)  & BITMASK(6)),
                                    *o++ = 128 | ( c        & BITMASK(6));
        } else {
            *o++ = 240 | (c >> 18), *o++ = 128 | ((c >> 12) & BITMASK(6)),
                                    *o++ = 128 | ((c >>  6) & BITMASK(6)),
                                    *o++ = 128 | ( c        & BITMASK(6));
        }
    }
    _8->p = realloc(_8->p, _8->s = o - _8->p);
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
    const size_t z = (a->l / 16 + 1) * 4 * (1 + (a->s > (1L << 32)));
    *b = *a, b->p = malloc(b->s), b->a = malloc(z);
    if (b->p == NULL || b->a == NULL) return 1;
    memcpy(b->p, a->p, b->s);
    memcpy(b->a, a->a,    z);
    return 0;
}

_Bool   vl_move_to_rstr( str_t *s, rstr_t *r) {
    if (s->s == 0) { *r = (rstr_t){}; return 0; }
    _Bool Z = s->s > (1L << 32);
    if ((r->a = malloc((s->l / 16 + 1) * 4 * (1 + Z))) == NULL) return 1;
    *(str_t *)r = *s; *s = (str_t){};
    const uint8_t *p = r->p, *const P = p + r->s;
    fiN(j, r->l / 16 + 1) {
        if (Z) ((uint64_t *)r->a)[j] = p - r->p; else ((uint32_t *)r->a)[j] = p - r->p;
        fiN(_, 16) { while (*p++ & 128); if (p == P) break; }
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
