#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "utf-vl.h"
#include <stdio.h>

#define BUFF_SIZE (1 << 22)

static uint8_t _in[BUFF_SIZE + 3];

int main(int argc, char **argv) {
    
    if (argc != 4 || !(!strcmp(argv[1], "-e") || !strcmp(argv[1], "-d"))) {
        
        printf("\n"
            
        "\tutf-vl -e some.utf-8  some.utf-vl\n"
        "\tutf-vl -d some.utf-vl some.utf-8\n"
            
        "\n"); return 1;
    }
    
    FILE *inf = fopen(argv[2], "rb"), *outf = fopen(argv[3], "wb"); struct stat instat;
    if (inf == NULL || fstat(fileno(inf), &instat) != 0 || outf == NULL) return 1;
    uint64_t insize = instat.st_size, tail = 0; uint8_t *in = _in;
    
    const _Bool encode = !strcmp(argv[1], "-e");
    
    while (insize > 0) {
        const uint64_t rsize = (insize < BUFF_SIZE) ? insize : BUFF_SIZE; insize -= rsize;
        if (fread(in, 1, rsize, inf) != rsize) return 1; junk_t _8; str_t _s;
        if (encode) {
            if (vl_8_from_bytes(_in, rsize + tail, &_8, &tail)
                || (insize == 0 && tail != 0)) return 1;
            if (vl_from_8(&_8, &_s)) return 1;
            if (fwrite(_s.p, 1, _s.s, outf) != _s.s) return 1; vl_free(&_s);
        } else {
            if (vl_from_bytes(_in, rsize + tail, &_s, &tail)
                || (insize == 0 && tail != 0)) return 1;
            if (vl_to_8(&_s, &_8)) return 1;
            if (fwrite(_8.p, 1, _8.s, outf) != _8.s) return 1; free(_8.p);
        }
        if (tail == 0) in = _in; else memmove(_in, (in + rsize) - tail, tail), in = _in + tail;
    }
    
    return (int)(fclose(inf) || fclose(outf));
}
