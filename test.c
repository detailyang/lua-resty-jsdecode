#include <stdio.h>
#include <stdlib.h>


size_t jsdecode(char *dst, char *src,  size_t src_len);


int
expect(char *ex, size_t ex_len, char *src, size_t len)
{
    char dst[1024] = {0};
    int  i = 0;

    size_t dst_len = jsdecode(dst, src, len);

    if (ex_len != dst_len) {
        printf("\"%s\" do not pass\n", src);
        exit(1);
    }

    for (i = 0; i < dst_len; i++) {
        if (ex[i] != dst[i]) {
            printf("\"%s\" do not pass\n", src);
            exit(1);
        }
    }

    printf("\"%s\" pass\n", src);

    return 1;
}


int
main() {
    char e0[] = {196, 163};
    expect(e0, 2, "\\u{123}", 7);

    char e1[] = {225, 136, 180};
    expect(e1, 3, "\\u{1234}", 8);

    char e2[] = {234, 175, 141, 196, 163};
    expect(e2, 5, "\\u{abcd}\\u{123}", 15);

    char e3[] = {92, 117, 123, 49, 50, 51};
    expect(e3, 6, "\\u{123", 6);

    char e4[] = {92, 117, 113};
    expect(e4, 3, "\\uq", 3);

    char e5[] = {225, 136, 177, 50, 51, 49, 50, 51, 49, 50, 51};
    expect(e5, 11, "\\u123123123123", 14);

    char e6[] = {92, 117, 123, 125};
    expect(e6, 4, "\\u{}", 4);

    char e7[] = {225, 136, 180, 53, 54, 55, 56, 57};
    expect(e7, 8, "\\u123456789", 11);

    char e8[] = {92, 117, 49};
    expect(e8, 3, "\\u1", 3);

    char e9[] = {92, 117, 49, 50};
    expect(e9, 4, "\\u12", 4);

    char e10[] = {92, 117, 123};
    expect(e10, 3, "\\u{", 3);

    char e11[] = {92, 117, 123, 49};
    expect(e11, 4, "\\u{1", 4);

    char e12[] = {92, 117, 123, 49, 119};
    expect(e12, 5, "\\u{1w", 5);

    char e13[] = {92, 117, 123, 49, 119, 125};
    expect(e13, 6, "\\u{1w}", 6);

    char e14[] = {92, 117, 123, 49, 97, 119, 99, 100, 101, 102, 103, 104, 105, 125};
    expect(e14, 14, "\\u{1awcdefghi}", 14);

    char e15[] = { 38, 108, 116, 59, 105, 109, 103, 32, 115, 114, 99, 61, 38, 113, 117, 111, 116, 59, 120, 120, 38, 113, 117, 111, 116, 59, 32, 47, 38, 103, 116, 59 };
    expect(e15, 32, "\\u0026\\u006c\\u0074\\u003b\\u0069\\u006d\\u0067\\u0020\\u0073\\u0072\\u0063\\u003d\\u0026\\u0071\\u0075\\u006f\\u0074\\u003b\\u0078\\u0078\\u0026\\u0071\\u0075\\u006f\\u0074\\u003b\\u0020\\u002f\\u0026\\u0067\\u0074\\u003b", 192);

    char ea[] = {83};
    expect(ea, 1, "\\123", 4);

    char eb[] = {169};
    expect(eb, 1, "\\251", 4);

    char ec[] = {1};
    expect(ec, 1, "\\1", 2);

    char ed[] = {0};
    expect(ed, 1, "\\0", 2);

    char ee[] = {92, 52};
    expect(ee, 2, "\\4", 2);

    char ef[] = {92, 52, 56};
    expect(ef, 3, "\\48", 3);

    char eg[] = {72};
    expect(eg, 1, "\\x48", 4);

    char eh[] = {72, 57};
    expect(eh, 2, "\\x489", 5);

    return 0;
}
