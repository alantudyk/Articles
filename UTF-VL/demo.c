#include <stdlib.h>
#include <string.h>
#include "utf-vl.h"

#define ps(s) vl_fwrite_as_8     (s, stdout)
#define pc(c) vl_fwrite_char_as_8(c, stdout)
#define pf printf
#define fin(stop) for (ssize_t i = -1, _stop = stop; ++i < _stop;)

int main(void) {
    
    uint8_t *j8[] = {
        
        "\n\tТекст.\n",
        
        "\n\tЕщё текст.\n\n"
        
    };
    
    const size_t jz = sizeof(j8) / 8; str_t s[jz];
    fin(jz) if (vl_from_8(&(junk_t){ j8[i], strlen(j8[i]) }, s + i)) return 1;
    
    fin(jz) if (ps(s + i)) return 1;
    
    return 0;
}
