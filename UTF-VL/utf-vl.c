#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>

_Static_assert(__SIZEOF_POINTER__ == 8, "");

#define BITMASK_SHL(n, shl) (((1LU << (n)) - 1) << (shl))
#define BITMASK(n) BITMASK_SHL(n, 0)

#define BUFF_SIZE (1lu << 22)

static  uint8_t S[256], _in[BUFF_SIZE + 7], _out[BUFF_SIZE * 4],
                V[8] = { 1, 2, 1, 3, 1, 2, 1, 0 };
static uint32_t L[5] = { 0,         0,     1 << 7,    1 << 11,    1 << 16  },
                M[5] = { 0, BITMASK(7), BITMASK(5), BITMASK(4), BITMASK(3) };

#define fin(stop) for (ssize_t i = -1, _stop = stop; ++i < _stop;)
#define fix(start, stop, increment) for (ssize_t i = start, _stop = stop; i < _stop; i += increment)

static void init_S() {
    fin(128)         S[i] = 1;
    fix(192, 224, 1) S[i] = 2;
    fix(224, 240, 1) S[i] = 3;
    fix(240, 248, 1) S[i] = 4;
}

int main(int argc, char **argv) {
    
    if (argc != 4 || !(!strcmp(argv[1], "-e") || !strcmp(argv[1], "-d"))) {
        
        printf("\n"
            
        "\tutf-vl -e some.utf-8  some.utf-vl\n"
        "\tutf-vl -d some.utf-vl some.utf-8\n"
            
        "\n"); return 1;
    }
    
    FILE *inf = fopen(argv[2], "rb"), *outf = fopen(argv[3], "wb"); struct stat instat;
    if (inf == NULL || fstat(fileno(inf), &instat) != 0 || outf == NULL) return 1;
    uint64_t insize = instat.st_size; init_S(); uint8_t *in = _in;
    
    _Bool encode = !strcmp(argv[1], "-e");
    
    while (insize > 0) {
        const uint64_t rsize = (insize < BUFF_SIZE) ? insize : BUFF_SIZE; insize -= rsize;
        if (fread(in, 1, rsize, inf) != rsize) return 1;
        const uint8_t *p = _in, *const P = in + rsize; *(uint32_t *)P = 0;
        uint8_t *o = _out;
        if (encode) {
            while (p < P) {
                uint8_t char_size = S[*p]; if (char_size == 0) return 1;
                if (p + char_size > P) {
                    if (insize == 0) return 1;
                    memmove(_in, p, P - p);
                    in = _in + (P - p);
                    break;
                }
                uint32_t c = *p++ & M[char_size];
                fix (1, char_size, 1) {
                    if ((*p >> 6) != 2) return 1;
                    c = (c << 6) | (*p++ & BITMASK(6));
                }
                if (c < L[char_size]) return 1;
                if (c < 128) { *o++ = c; continue; }
                c -= 128;
                if (c < (1 << 14)) {
                    *o++ = 128 | (c & BITMASK(7)), *o++ = c >> 7;
                    continue;
                }
                c -= 1 << 14;
                *o++ = 128 | (c & BITMASK(7)), *o++ = 128 | ((c >> 7) & BITMASK(7)), *o++ = c >> 14;
            }
            if (p == P) in = _in; 
        } else {
            while (p < P) {
                uint8_t char_size = V[(*p >> 7) | ((p[1] >> 7) << 1) | ((p[2] >> 7) << 2)];
                if (char_size == 0) return 1;
                if (p + char_size > P) {
                    if (insize == 0) return 1;
                    memmove(_in, p, P - p);
                    in = _in + (P - p);
                    break;
                }
                uint32_t c = *p++ & BITMASK(7);
                fix(1, char_size, 1) c |= (*p++ & BITMASK(7)) << (7 * i);
                switch(char_size) {
                    case 3: c += 1 << 14;
                    case 2: c += 128;
                }
                if (c < 128) *o++ = c; else if (c < (1 << 11)) {
                    *o++ = 192 | (c >>  6), *o++ = 128 + (c & BITMASK(6));
                } else if (c < (1 << 16)) {
                    *o++ = 224 | (c >> 12), *o++ = 128 + ((c >> 6) & BITMASK(6)),
                                            *o++ = 128 + (c & BITMASK(6));
                } else {
                    *o++ = 240 | (c >> 18), *o++ = 128 + ((c >> 12) & BITMASK(6)),
                                            *o++ = 128 + ((c >>  6) & BITMASK(6)),
                                            *o++ = 128 + (c & BITMASK(6));
                }
            }
            if (p == P) in = _in;
        }
        if (fwrite(_out, 1, o - _out, outf) != o - _out) return 1;
    }
    
    return (int)(fclose(inf) || fclose(outf));
}
