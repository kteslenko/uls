#include "libmx.h"

static int byte_len(wchar_t c) {
    static const wchar_t last[] = {0x7f, 0x7ff, 0xffff, 0x10ffff};
    
    for (int i = 0; i < 4; i++) {
        if (c <= last[i]) {
            return i + 1;
        }
    }

    return 4;
}

void mx_print_unicode(wchar_t c) {
    static const wchar_t mask[] = {0x7f, 0x1f, 0xf, 0x7};
    static const wchar_t msb[] = {0x0, 0xc0, 0xe0, 0xf0};
    unsigned char utf8[4];
    int len = byte_len(c);

    for (int i = len - 1; i > 0; i--) {
        utf8[i] = (c & 0x3f) | 0x80;
        c >>= 6;
    }

    utf8[0] = (c & mask[len - 1]) | msb[len - 1];
    write(1, utf8, len);
}
