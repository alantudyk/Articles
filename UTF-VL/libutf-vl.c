#include <stdlib.h>
#include <string.h>
#include "utf-vl.h"

#define BITMASK_SHL(n, shl) (((1LU << (n)) - 1) << (shl))
#define BITMASK(n) BITMASK_SHL(n, 0)

#define fin(stop) for (ssize_t i = -1, _stop = stop; ++i < _stop;)
#define fiN(iter_name, stop) for (ssize_t iter_name = -1, _stop = stop; ++iter_name < _stop;)
#define fix(start, stop, increment) \
    for (ssize_t i = start, _stop = stop; i < _stop; i += increment)
#define fiX(iter_name, start, stop, increment) \
    for (ssize_t iter_name = start, _stop = stop; iter_name < _stop; iter_name += increment)

 void vl_init(vl_iterator_t *i, const str_t *s) {
    *i = (vl_iterator_t){ s->p, s->l };
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

static const uint32_t junk_L[5] = { 0,         0 ,    1 << 7 ,   1 << 11 ,   1 << 16  },
                      junk_M[5] = { 0, BITMASK(7), BITMASK(5), BITMASK(4), BITMASK(3) };

_Bool vl_8_from_bytes(const uint8_t *p, size_t s, junk_t *_dest, size_t *tail) {
    if (s == 0) return 1; const uint8_t *const P = p + s; *tail = 0;
    *_dest = (junk_t){ (uint8_t *)p, s };
    while (p < P) {
        uint8_t char_size = junk_S[*p]; if (char_size == 0) return 1;
        if (p + char_size > P) {
            if ((*tail = P - p) == s) return 1;
            _dest->s -= *tail;
            return 0;
        }
        uint32_t c = *p++ & junk_M[char_size];
        fix (1, char_size, 1) {
            if ((*p >> 6) != 2) return 1;
            c = (c << 6) | (*p++ & BITMASK(6));
        }
        if (c < junk_L[char_size] || c > 1114111) return 1;
    }
    return 0;
}

_Bool   vl_from_bytes(const uint8_t *p, size_t s,  str_t *_dest, size_t *tail) {
    const uint8_t *t = p + s, *P = t;
    fin(3) { if (P == p) return 1; if (P[-1] & 128) --P; else break; }
    if ((*tail = t - P) == 3) return 1; P -= 2;
    *_dest = (str_t){ (uint8_t *)p, s - *tail, 0 };
    while (p < P) {
        uint8_t c[4] = {}; _dest->l++;
        if (((c[0] = *p++) & 128) && ((c[1] = *p++) & 128)) {
            c[2] = *p++; if (*(uint32_t *)c > 4390655) return 1;
        }
    }
    switch ((P + 2) - p) {
        case 1: _dest->l++; break;
        case 2: _dest->l += (*p & 128) ? 1 : 2;
    }
    return 0;
}

void vl_free(str_t *s) { free(s->p); /* free(s->a); */ *s = (str_t){}; }

size_t vl_char_size(const int32_t *c) {
    const uint8_t *p = (const uint8_t *)c;
    while (*p++ & 128);
    return (size_t)(p - (const uint8_t *)c);
}

_Bool vl_char_at(const str_t *s, const size_t _i, int32_t *_c) {
    if (  _i >= s->l) return 1;
    if (s->l == s->s) *_c = *(s->p + _i); else {
        const uint8_t *p = s->p + (s->s > (1L << 32) ? ((uint64_t *)s->a)[_i / 16] :
                                                       ((uint32_t *)s->a)[_i / 16]);
        uint8_t *c = (uint8_t *)_c; *_c = 0;
        fin(_i % 16) while (*p++ & 128);
        do *c++ = *p; while (*p++ & 128);
    }
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

_Bool vl_equal(const str_t *a, const str_t *b) {
    return a == b || (a->s == b->s && a->l == b->l && !memcmp(a->p, b->p, a->s));
}

 int   vl_cmp (const str_t *a, const str_t *b) {
    if (a->l > b->l) { const void *t = a; a = b, b = t; }
    const uint8_t *pa = a->p, *pb = b->p;
    if (a->l == a->s && b->l == b->s)
        fin(a->l) if (pa[i] != pb[i]) return pa[i] - pb[i];
    else
        fiN(_, a->l) {
            int32_t ca = 0, cb = 0;
            uint8_t *cpa = (void *)&ca, *cpb = (void *)&cb;
            do *cpa++ = *pa; while (*pa++ & 128);
            do *cpb++ = *pb; while (*pb++ & 128);
            if (ca != cb) return ca - cb;
        }
    return -(b->l > a->l);
}

_Bool vl_from_8(const junk_t *_8,  str_t *_s) {
    if (_8->s == 0) { *_s = ( str_t){}; return 0; }
    uint8_t *o = _s->p = malloc(_8->s); if (o == NULL) return 1;
    const uint8_t *p = _8->p, *const P = p + _8->s; _s->l = 0;
    while (p < P) {
        uint32_t char_size = junk_S[*p], c = *p++ & junk_M[char_size]; _s->l++;
        fix (1, char_size, 1) c = (c << 6) | (*p++ & BITMASK(6));
        if (c < 128) *o++ = c;
        else if ((c -= 128) < (1 << 14))  *o++ = 128 | c, *o++ = c >> 7;
        else *o++ = 128 | (c -= 1 << 14), *o++ = 128 | (c >> 7), *o++ = c >> 14;
    }
    _s->p = realloc(_s->p, _s->s = o - _s->p);
    return 0;
}

_Bool   vl_to_8(const  str_t *_s, junk_t *_8) {
    if (_s->s == 0) { *_8 = (junk_t){}; return 0; }
    uint8_t *o = _8->p = malloc(_s->s + _s->s / 2 + 1); if (o == NULL) return 1;
    const uint8_t *p = _s->p, *const P = p + _s->s;
    while (p < P) {
        uint32_t c = *p & BITMASK(7);
        if (*p++ & 128) {
            c |= (*p & BITMASK(7)) << 7;
            if (*p++ & 128)
                c |= *p++ << 14, c += (1 << 14) + 128;
            else c += 128;
        }
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

void vl_move(str_t *a, str_t *b) { *b = *a; *a = (str_t){}; }

_Bool vl_clone(const str_t *a, str_t *b) {
    if (a->s == 0) { *b = (str_t){}; return 0; } *b = *a;
    if ((b->p = malloc(b->s)) == NULL) { *b = (str_t){}; return 1; }
    memcpy(b->p, a->p, b->s);
    if (b->a != NULL) {
        const size_t z = (b->l / 16 + 1) * 4 * (1 + (b->s > (1L << 32)));
        if ((b->a = malloc(z)) == NULL) { vl_free(b); return 1; }
        memcpy(b->a, a->a, z);
    }
    return 0;
}

_Bool vl_fwrite_char_as_8(int32_t c, FILE *f) {
    
    return 0;
}

_Bool vl_fwrite_as_8(const str_t *s, FILE *f) {
    junk_t _8 = {}; _Bool r = vl_to_8(s, &_8) || fwrite(_8.p, 1, _8.s, f) != _8.s;
    free(_8.p); return r;
}
