#include <string.h>
#include <stdio.h>

#define IS_HEX(c) ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
#define AXTOI(c) ((c >= '0' && c <= '9') ? (c - '0') : \
                    ((c >= 'a' && c <= 'f') ? (c - 'a') + 10 : \
                        (c - 'A') + 10))


static int
unicode_to_utf8(char *dst, const char *src, int width)
{
    int codepoint;
    int i;
    
    for (i = 0; i < width; i++) {
        if (!IS_HEX(src[i])) return 0;
    }
    
    for (i = 0, codepoint = 0; i < width; i++) {
        codepoint += AXTOI(src[i]) << ((width - i - 1) * 4);
    }
    
    if (codepoint <= 0x007F) {
        dst[0] = codepoint;
        return 1;
        
    } else if (codepoint <= 0x07FF) {
        dst[0] = 0xC0 | ((codepoint >> 6) & 0x1F);
        dst[1] = 0x80 | (codepoint & 0x3F);
        return 2;
        
    } else if (codepoint <= 0xffff) {
        dst[0] = 0xE0 | ((codepoint >> 12) & 0xF);
        dst[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        dst[2] = 0x80 | (codepoint & 0x3F);
        return 3;
        
    } else if (codepoint <= 0x10FFFF) {
        dst[0] = 0xF0 | ((codepoint >> 18) & 0x7);
        dst[1] = 0x80 | ((codepoint >> 12) & 0x3F);
        dst[2] = 0x80 | ((codepoint >> 6) & 0x3F);
        dst[3] = 0x80 | (codepoint & 0x3F);
        return 4;
    }    
    
    return 0;
}


size_t
jsdecode(char *dst, const char *src,  size_t src_len)
{
    int     bytes, width;
    char    *guard, *dp;
    size_t  i;
    
    guard = dst;
    dp = dst;
    
    for (i = 0; i < src_len; i++) {
        if (src[i] == '\\') {
            if (i + 1 < src_len) {
                /* peek one byte */
                switch( src[++i]) {
                    case '0':
                        *dp++ = 0;
                        break;
                    case '\'':
                        *dp++ = 39;
                        break;
                    case '\"':
                        *dp++ = 34;
                        break;
                    case '\\':
                        *dp++ = 92;
                        break;
                    case 'n':
                        *dp++ = 10;
                        break;
                    case 'r':
                        *dp++ = 13;
                        break;
                    case 'v':
                        *dp++ = 11;
                        break;
                    case 't':
                        *dp++ = 9;
                        break;
                    case 'b':
                        *dp++ = 8;
                        break;
                    case 'f':
                        *dp++ = 12;
                        break;
                    case 'u':
                        if (i + 1 > src_len) {
                            continue;
                        }
                        /* peek one byte */
                        if (src[i+1] != '{') {
                            /* peek four bytes */
                            if (src_len - i - 1 < 4) {
                                *dp++ = '\\';
                                *dp++ = 'u';
                                
                                if (src_len - i == 1) {
                                    continue;
                                    
                                } else if (src_len - i == 2) {
                                    *dp++ = src[++i];
                                    
                                } else if (src_len - i == 3){
                                    *dp++ = src[++i];
                                    *dp++ = src[++i];
                                    
                                /* == 4 */
                                } else {
                                    *dp++ = src[++i];
                                    *dp++ = src[++i];
                                    *dp++ = src[++i];
                                }
                                
                            } else {
                                bytes = unicode_to_utf8(dp, src + i + 1, 4);
                                if (bytes == 0) {
                                    *dp++ = '\\';
                                    *dp++ = 'u';    
                                    *dp++ = src[++i];
                                    *dp++ = src[++i];
                                    *dp++ = src[++i];
                                    *dp++ = src[++i];
                                } else {
                                    i += 4;
                                    dp += bytes;
                                }
                            }

                        } else {
                            /* lookup } */
                            i = i + 1; /* skip { */
                            width = 0;
                            
                            if (i + 1 < src_len && src[i+1] == '}') {
                                width = 0;
                            } else if (i + 2 < src_len && src[i+2] == '}'){
                                width = 1;
                            } else if (i + 3 < src_len && src[i+3] == '}'){
                                width = 2;
                            } else if (i + 4 < src_len && src[i+4] == '}') {
                                width = 3;
                            } else if (i + 5 < src_len && src[i+5] == '}') {
                                width = 4;
                            } else if (i + 6 < src_len && src[i+6] == '}'){
                                width = 5;
                            }
                            
                            if (width >= 1 && width <= 6) {
                                i++;
                                bytes = unicode_to_utf8(dp, src + i, width);
                                if (bytes == 0) {
                                    *dp++ = '\\';
                                    *dp++ = 'u';    
                                    *dp++ = '{';
                                    
                                    while(width--) {
                                        *dp++ = src[i++];
                                    }
                                    
                                    *dp++ = '}';
                                    i++;
                                    
                                } else {
                                    i += width;
                                    dp += bytes;
                                }
                                
                            } else if (width == 0) {
                                *dp++ = '\\';
                                *dp++ = 'u';    
                                *dp++ = '{';
                                
                            } else {
                                *dp++ = '\\';
                                *dp++ = 'u';    
                                *dp++ = '{';
                                
                                while(width--) {
                                    *dp++ = src[++i];
                                }
                                
                                *dp++ = '}';    
                                i++;
                            }
                        }
                        
                        break;
                        
                    default:
                        *dp++ = '\\';
                        *dp++ = src[i - 1];
                } 
            }
        } else {
            *dp++ = src[i];
        }
    }
    
    return (size_t )(dp - guard);
}

