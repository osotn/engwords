/*
 * English words
 * by Oleksandr Sotnikov
 *
 * Utility: Translate UTF-8 cirillic to oem866
 *    utf8_ciril_to_oem866
 * 
 * (c) 2021
 */

#include <stdio.h>

int utf8_is_cirillic(int c)
{
/*
 *       U+0080..U+07FF: 110xxxxx 10xxxxxx
 *
 *       110.....  10......
 *       U+0x400 = 0100 0000 0000 
 *          10000    000000
 *       -------------------
 *       1101ZZZC   10CCCCCC
 *        D0-D1         
 */
        return (c == 0xD0 || c == 0xD1);
}

int utf8_get_u_2byte(int c1, int c2)
{
    /*
     *  U+0080..U+07FF: 110xxxxx 10xxxxxx
     */
    return ((c1 & 0x1F) << 6) +  (c2 & 0x3F);
}

int main()
{
    int c;
    while ((c = getchar()) != EOF)
    {
        if (utf8_is_cirillic(c))
        {
            int c2 = getchar();
            int u = utf8_get_u_2byte(c, c2);
            int oem866 = '?';
 
            //printf("{%2x}", u);
            if (0x410 <= u && u <= 0x43f)
                 oem866 = u - 0x400 + 0x70;
            else if ((0x440 <= u && u <= 0x44f) || (u == 0x451))
                 oem866 = u - 0x400 + 0xA0;
            else if (u == 0x401)
                 oem866 = 0xF0;
            //printf("[%2x]", oem866);
            putchar(oem866);
        } 
        else
        {
            putchar(c);
        }      
    }

    return 0;
}

