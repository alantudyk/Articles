#include <string.h>
#include "utf-vl.h"

#define ps(s) vl_fwrite_as_8     (s, stdout)
#define pc(c) vl_fwrite_char_as_8(c, stdout)
#define pf printf
#define fin(stop) for (ssize_t i = -1, _stop = stop; ++i < _stop;)

int cmp(void *a, void *b) { return vl_cmp(*(const str_t **)a, *(const str_t **)b); }

int main(void) {
    
    uint8_t *j8[] = {
        "Текст",
        "Текстище",
    };
    
    const size_t jz = sizeof(j8) / 8; str_t s[jz];
    fin(jz) if (vl_from_8(&(junk_t){ j8[i], strlen(j8[i]) }, s + i)) return 1;
    
    pf("\n\tPrinting two strings vertically using `vl_iterator_t`:\n\n");
    
    vl_iterator_t iter[2]; fin(2) vl_init(iter + i, s + i); int32_t c[2];
    
    fin(s[0].l > s[1].l ? s[0].l : s[1].l) {
        fin(2) if (vl_next(iter + i, c + i)) c[i] = ' ';
        pf("\t\t"); pc(c[0]); pf("\t"); pc(c[1]); pf("\n");
    }
    
    pf("\n");
    
    return 0;
}
