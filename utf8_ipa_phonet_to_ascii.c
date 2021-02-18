/*
 * English words
 * by Oleksandr Sotnikov
 *
 * Utility: Translate IPA English phonetics to ASCII code
 *    utf8_ipa_phonet_to_ascii
 * 
 * (c) 2021
 */

#include <stdio.h>

int utf8_is_u_2byte(int c)
{
/*
 *       U+0080..U+07FF: 110xxxxx 10xxxxxx
 */
        return (c & ~0x1F) == 0xC0;
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
        if (utf8_is_u_2byte(c))
        {
            int c2 = getchar();
            int u = utf8_get_u_2byte(c, c2);

            switch (u)
            {
            case 0x0E6: /* æ  */  printf("@");     break;
            case 0x0F0: /* ð  */  printf("<Z>");   break;
            case 0x14B: /* ŋ  */  printf("<n>");   break;
            case 0x251: /* ɑ  */  printf("a");     break; 
            case 0x252: /* ɒ  */  printf("<o>");   break;
            case 0x254: /* ɔ  */  printf("o");     break;   
            case 0x259: /* ə  */  printf("<a>");   break;
            case 0x25B: /* ɛ  */  printf("e");     break;
            case 0x25C: /* ɜ  */  printf("<e>");   break;
            case 0x26A: /* ɪ  */  printf("<i>");   break;
            case 0x283: /* ʃ  */  printf("<sh>");  break;
            case 0x28A: /* ʊ  */  printf("<u>");   break;
            case 0x28C: /* ʌ  */  printf("^");     break;
            case 0x292: /* ʒ  */  printf("<z>");   break; 
            case 0x2A4: /* ʤ  */  printf("<dz>");  break;
            case 0x2A7: /* ʧ  */  printf("<tch>"); break;
            case 0x2C8: /* ˈ  */  printf("'");     break;
            case 0x2CC: /* ˌ  */  printf(",");     break;
            case 0x2D0: /* ː  */  printf(":");     break;
            case 0x3B8: /* θ  */  printf("<S>");   break;
            default:
                putchar(c);
                putchar(c2);
                break;
            }
        } 
        else
        {
            putchar(c);
        }      
    }

    return 0;
}
