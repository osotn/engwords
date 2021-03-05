/*
 * English words
 * by Oleksandr Sotnikov.
 *
 * This utility translates utf-8:
 *  - IPA English phonetics to POS58 user-defined characters;
 *  - cirillic to OEM866 code page.
 *
 * (c) 2021
 */

#include <stdio.h>

int utf8_is_u_2byte(int c)
{
    /*
     *  U+0080..U+07FF: 110xxxxx 10xxxxxx
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

int utf8_is_cirillic(int c)
{
/*
 *       UTF-8 2 bytes format:
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

void define_use_character_12_24_cmd(char bitmap[], int bitmap_size, char user_char)
{
    int i;

    putchar('\x1B');
    putchar('&');
    putchar('\x03');     // H = 24 bits
    putchar(user_char);
    putchar(user_char);
    putchar('\x0B');     // W = 12 bits

    for (i = 0; i < bitmap_size; i++)
        putchar(bitmap[i]);
}

void enable_use_characters_mode_cmd(void)
{
    putchar('\x1B');
    putchar('\x25');
    putchar('\x01');
}

void disable_use_characters_mode_cmd(void)
{
    putchar('\x1B');
    putchar('\x25');
    putchar('\x00');
}

void put_user_char(char uc)
{
    enable_use_characters_mode_cmd();
    putchar(uc);
    disable_use_characters_mode_cmd();
}

int main()
{
    int c;

    /* Test */
    char IPA_Utest_bitmap[36] = {
/*
* * * * * * 
 * * * * * *
* * * * * * 
 * * * * * *
* * * * * * 
 * * * * * *
* * * * * * 
 * * * * * *
* * * * * * 
 * * * * * *
* * * * * * 
 * * * * * *
* * * * * * 
 * * * * * *
* * * * * * 
 * * * * * *
* * * * * * 
 * * * * * *
* * * * * * 
 * * * * * *
* * * * * * 
 * * * * * *
* * * * * * 
 * * * * * *
*/
        '\xAA', '\xAA', '\xAA',
        '\x55', '\x55', '\x55',
        '\xAA', '\xAA', '\xAA',
        '\x55', '\x55', '\x55',
        '\xAA', '\xAA', '\xAA',
        '\x55', '\x55', '\x55',
        '\xAA', '\xAA', '\xAA',
        '\x55', '\x55', '\x55',
        '\xAA', '\xAA', '\xAA',
        '\x55', '\x55', '\x55',
        '\xAA', '\xAA', '\xAA',
        /*'\x55', '\x55', '\x55',*/ '\x00', '\x00', '\x00', /* <--- Must be \x00 */
    };

    /* ɛ  */
    char IPA_U025B_bitmap[36] = {
/*
            
            
            
            
            
  ******    
 ********   
 **    ***  
**          
**          
 **         
 **         
  *****     
  *****     
 **         
 **         
**          
**          
 **    ***  
 ********   
  ******    
            
            

*/            
        '\x00', '\xC0', '\xC0',
        '\x03', '\xF3', '\xF0',
        '\x07', '\x3F', '\x38',
        '\x06', '\x0C', '\x18',
        '\x06', '\x0C', '\x18',
        '\x06', '\x0C', '\x18',
        '\x06', '\x0C', '\x18',
        '\x07', '\x00', '\x38',
        '\x03', '\x00', '\x30',
        '\x01', '\x00', '\x20',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* ɜ  */
    char IPA_U025C_bitmap[36] = {
/*
            
            
            
            
            
  ******    
 ********   
***    **   
        **  
        **  
       **   
       **   
   *****    
   *****    
       **   
       **   
        **  
        **  
***    **   
 ********   
  ******    
            
            
            
*/
        '\x01', '\x00', '\x20',
        '\x03', '\x00', '\x30',
        '\x07', '\x00', '\x38',
        '\x06', '\x0C', '\x18',
        '\x06', '\x0C', '\x18',
        '\x06', '\x0C', '\x18',
        '\x06', '\x0C', '\x18',
        '\x07', '\x3F', '\x38',
        '\x03', '\xF3', '\xF0',
        '\x00', '\xC0', '\xC0',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* ː */
    char IPA_U02D0_bitmap[36] = {
/*
            
            
            
            
            
            
******      
 ****       
  **        
            
            
            
            
            
            
            
            
            
  **        
 ****       
******      
            
            
            
*/
        '\x02', '\x00', '\x08',
        '\x03', '\x00', '\x18',
        '\x03', '\x80', '\x38',
        '\x03', '\x80', '\x38',
        '\x03', '\x00', '\x18',
        '\x02', '\x00', '\x08',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* θ */
    char IPA_U03B8_bitmap[36] = {
/*
            
   ****     
  ******    
 **    **   
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**********  
**********  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
 **    **   
  ******    
   ****     
            
            
            
*/
        '\x0F', '\xFF', '\xC0',
        '\x1F', '\xFF', '\xE0',
        '\x30', '\x30', '\x30',
        '\x60', '\x30', '\x18',
        '\x60', '\x30', '\x18',
        '\x60', '\x30', '\x18',
        '\x60', '\x30', '\x18',
        '\x30', '\x30', '\x30',
        '\x1F', '\xFF', '\xE0',
        '\x0F', '\xFF', '\xC0',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* ə */
    char IPA_U0259_bitmap[36] = {
        '\x01', '\x8F', '\xE0',
        '\x03', '\x8F', '\xF0',
        '\x07', '\x0C', '\x38',
        '\x06', '\x0C', '\x18',
        '\x06', '\x0C', '\x18',
        '\x06', '\x0C', '\x18',
        '\x06', '\x0C', '\x18',
        '\x07', '\x0C', '\x38',
        '\x03', '\xFF', '\xF0',
        '\x01', '\xFF', '\xE0',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* ɔ */
    char IPA_U0254_bitmap[36] = {
/*

                        
            
            
            
  ******    
 ********   
***    ***  
**      **  
        **  
        **  
        **  
        **  
        **  
        **  
        **  
        **  
**      **  
***    ***  
 ********   
  ******    
            
            
            
*/
        '\x01', '\x80', '\x60',
        '\x03', '\x80', '\x70',
        '\x07', '\x00', '\x38',
        '\x06', '\x00', '\x18',
        '\x06', '\x00', '\x18',
        '\x06', '\x00', '\x18',
        '\x06', '\x00', '\x18',
        '\x07', '\x00', '\x38',
        '\x03', '\xFF', '\xF0',
        '\x01', '\xFF', '\xE0',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* ɒ */
    char IPA_U0252_bitmap[36] = {
/*
            
            
            
            
            
** *****    
*********   
***    ***  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
***    ***  
*********   
** *****    
            
            
            
*/
        '\x07', '\xFF', '\xF8',
        '\x07', '\xFF', '\xF8',
        '\x03', '\x00', '\x30',
        '\x06', '\x00', '\x18',
        '\x06', '\x00', '\x18',
        '\x06', '\x00', '\x18',
        '\x06', '\x00', '\x18',
        '\x07', '\x00', '\x38',
        '\x03', '\xFF', '\xF0',
        '\x01', '\xFF', '\xE0',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* ɑ */
    char IPA_U0251_bitmap[36] = {
/*
            
            
            
            
            
  ***** **  
 *********  
***    ***  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
***    ***  
 *********  
  ***** **  
            
            
            
*/
        '\x01', '\xFF', '\xE0',
        '\x03', '\xFF', '\xF0',
        '\x07', '\x00', '\x38',
        '\x06', '\x00', '\x18',
        '\x06', '\x00', '\x18',
        '\x06', '\x00', '\x18',
        '\x06', '\x00', '\x18',
        '\x03', '\x00', '\x30',
        '\x07', '\xFF', '\xF8',
        '\x07', '\xFF', '\xF8',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* ŋ */
    char IPA_U014B_bitmap[36] = {
/*
            
            
            
            
            
            
** *****    
*********   
****   ***  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
       ***  
     ****   
     ***    
*/
        '\x03', '\xFF', '\xF8',
        '\x03', '\xFF', '\xF8',
        '\x01', '\x80', '\x00',
        '\x03', '\x80', '\x00',
        '\x03', '\x00', '\x00',
        '\x03', '\x00', '\x03',
        '\x03', '\x00', '\x03',
        '\x03', '\x80', '\x07',
        '\x01', '\xFF', '\xFE',
        '\x00', '\xFF', '\xFC',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* ð */
    char IPA_U00F0_bitmap[36] = {
/*
            
  **   **   
   ** **    
    ***     
    ***     
   ** **    
  **   **   
       **   
        **  
        **  
  ********  
 *********  
***     **  
**      **  
**      **  
**      **  
**      **  
**      **  
 **    **   
  ******    
   ****     
            
            
            
*/
        '\x00', '\x0F', '\xC0',
        '\x00', '\x1F', '\xE0',
        '\x42', '\x38', '\x30',
        '\x66', '\x30', '\x18',
        '\x3C', '\x30', '\x18',
        '\x18', '\x30', '\x18',
        '\x3C', '\x30', '\x18',
        '\x67', '\x30', '\x30',
        '\x43', '\xFF', '\xE0',
        '\x00', '\xFF', '\xC0',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* æ */
    char IPA_U00E6_bitmap[36] = {
/*
            
            
            
            
            
 ***  **    
*********   
**  ** ***  
    **  **  
    **  **  
    **  **  
    ** ***  
  ********  
 ********   
*** **      
**  **      
**  **      
**  **      
*** **  **  
 *********  
  **  ***   
            
            
            
*/
        '\x03', '\x03', '\xE0',
        '\x07', '\x07', '\xF0',
        '\x06', '\x0E', '\x38',
        '\x06', '\x0C', '\x18',
        '\x03', '\xFF', '\xF0',
        '\x03', '\xFF', '\xF0',
        '\x06', '\x0C', '\x18',
        '\x07', '\x1C', '\x18',
        '\x03', '\xFC', '\x38',
        '\x01', '\xF8', '\x30',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* ɪ */
    char IPA_U026A_bitmap[36] = {
/*
            
            
            
            
            
  ******    
  ******    
    **      
    **      
    **      
    **      
    **      
    **      
    **      
    **      
    **      
    **      
    **      
    **      
  ******    
  ******    
            
            
            
*/
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
        '\x06', '\x00', '\x18',
        '\x06', '\x00', '\x18',
        '\x07', '\xFF', '\xF8',
        '\x07', '\xFF', '\xF8',
        '\x06', '\x00', '\x18',
        '\x06', '\x00', '\x18',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* ʃ */
    char IPA_U0283_bitmap[36] = {
/*
            
      ***   
     *****  
    *** **  
    **      
    **      
    **      
    **      
    **      
    **      
    **      
    **      
    **      
    **      
    **      
    **      
    **      
    **      
    **      
    **      
    **      
** ***      
*****       
 ***        
*/
        '\x00', '\x00', '\x06',
        '\x00', '\x00', '\x07',
        '\x00', '\x00', '\x03',
        '\x00', '\x00', '\x07',
        '\x1F', '\xFF', '\xFE',
        '\x3F', '\xFF', '\xFC',
        '\x70', '\x00', '\x00',
        '\x60', '\x00', '\x00',
        '\x70', '\x00', '\x00',
        '\x30', '\x00', '\x00',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* ʧ */
    char IPA_U02A7_bitmap[36] = {
/*
            
       ***  
 **   ***** 
 **  *** ** 
 **  **     
 **  **     
 **  **     
 **  **     
*******     
*******     
 **  **     
 **  **     
 **  **     
 **  **     
 **  **     
 **  **     
 **  **     
 ******     
  *****     
     **     
     **     
 ** ***     
 *****      
  ***       
*/
        '\x00', '\xC0', '\x00',
        '\x3F', '\xFF', '\xC6',
        '\x3F', '\xFF', '\xE7',
        '\x00', '\xC0', '\x63',
        '\x00', '\xC0', '\x67',
        '\x1F', '\xFF', '\xFE',
        '\x3F', '\xFF', '\xFC',
        '\x70', '\x00', '\x00',
        '\x60', '\x00', '\x00',
        '\x70', '\x00', '\x00',
        '\x30', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* ʊ */
    char IPA_U028A_bitmap[36] = {
/*
            
            
            
            
            
****  ****  
****  ****  
  **  **    
  **  **    
 ***  ***   
***    ***  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
**      **  
***    ***  
 ********   
  ******    
*/
        '\x06', '\x3F', '\xE0',
        '\x06', '\x7F', '\xF0',
        '\x07', '\xE0', '\x38',
        '\x07', '\xC0', '\x18',
        '\x00', '\x00', '\x18',
        '\x00', '\x00', '\x18',
        '\x07', '\xC0', '\x18',
        '\x07', '\xE0', '\x38',
        '\x06', '\x7F', '\xF0',
        '\x06', '\x3F', '\xE0',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* ʒ */
    char IPA_U0292_bitmap[36] = {

/*
            
            
            
            
            
**********  
**********  
       ***  
      ***   
     ***    
    ***     
   ***      
  *****     
  ******    
      ***   
       ***  
        **  
        **  
        **  
        **  
*      ***  
**    ***   
********    
 ******     
*/
        '\x06', '\x00', '\x0E',
        '\x06', '\x00', '\x07',
        '\x06', '\x0C', '\x03',
        '\x06', '\x1C', '\x03',
        '\x06', '\x3C', '\x03',
        '\x06', '\x7C', '\x03',
        '\x06', '\xEE', '\x07',
        '\x07', '\xC7', '\x0E',
        '\x07', '\x83', '\xFC',
        '\x07', '\x01', '\xF8',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
    };

    /* ʌ */
    char IPA_U028C_bitmap[36] = {
/*
            
            
            
            
            
    **      
    **      
   ****     
   ****     
  **  **    
  **  **    
  **  **    
  **  **    
 **    **   
 **    **   
 **    **   
 **    **   
**      **  
**      **  
**      **  
**      **  
            
            
            
*/
        '\x00', '\x00', '\x78',
        '\x00', '\x07', '\xF8',
        '\x00', '\x7F', '\x80',
        '\x01', '\xF8', '\x00',
        '\x07', '\x80', '\x00',
        '\x07', '\x80', '\x00',
        '\x01', '\xF8', '\x00',
        '\x00', '\x7F', '\x80',
        '\x00', '\x07', '\xF8',
        '\x00', '\x00', '\x78',
        '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00',
   };

    char IPA_Utest_user_char = '!';    /* test */
    char IPA_U00E6_user_char = '\"';   /* æ */
    char IPA_U00F0_user_char = '#';    /* ð */
    char IPA_U014B_user_char = '$';    /* ŋ */
    char IPA_U0251_user_char = '%';    /* ɑ */
    char IPA_U0252_user_char = '&';    /* ɒ */ 
    char IPA_U0254_user_char = '\'';   /* ɔ */ 
    char IPA_U0259_user_char = '(';    /* ə */ 
    char IPA_U025B_user_char = ')';    /* ɛ */ 
    char IPA_U025C_user_char = '*';    /* ɜ */ 
    char IPA_U026A_user_char = '+';    /* ɪ */
    char IPA_U0283_user_char = ',';    /* ʃ */
    char IPA_U028A_user_char = '-';    /* ʊ */
    char IPA_U028C_user_char = '.';    /* ʌ */
    char IPA_U0292_user_char = '/';    /* ʒ */
    char IPA_U02A7_user_char = '0';    /* ʧ */
    char IPA_U02D0_user_char = '1';    /* ː */ 
    char IPA_U03B8_user_char = '2';    /* θ */ 

    define_use_character_12_24_cmd(IPA_Utest_bitmap, sizeof(IPA_Utest_bitmap), IPA_Utest_user_char);
    define_use_character_12_24_cmd(IPA_U00E6_bitmap, sizeof(IPA_U00E6_bitmap), IPA_U00E6_user_char);
    define_use_character_12_24_cmd(IPA_U00F0_bitmap, sizeof(IPA_U00F0_bitmap), IPA_U00F0_user_char);
    define_use_character_12_24_cmd(IPA_U014B_bitmap, sizeof(IPA_U014B_bitmap), IPA_U014B_user_char);
    define_use_character_12_24_cmd(IPA_U0251_bitmap, sizeof(IPA_U0251_bitmap), IPA_U0251_user_char);
    define_use_character_12_24_cmd(IPA_U0252_bitmap, sizeof(IPA_U0252_bitmap), IPA_U0252_user_char);
    define_use_character_12_24_cmd(IPA_U0254_bitmap, sizeof(IPA_U0254_bitmap), IPA_U0254_user_char);
    define_use_character_12_24_cmd(IPA_U0259_bitmap, sizeof(IPA_U0259_bitmap), IPA_U0259_user_char);
    define_use_character_12_24_cmd(IPA_U025B_bitmap, sizeof(IPA_U025B_bitmap), IPA_U025B_user_char);
    define_use_character_12_24_cmd(IPA_U025C_bitmap, sizeof(IPA_U025C_bitmap), IPA_U025C_user_char);
    define_use_character_12_24_cmd(IPA_U026A_bitmap, sizeof(IPA_U026A_bitmap), IPA_U026A_user_char);
    define_use_character_12_24_cmd(IPA_U0283_bitmap, sizeof(IPA_U0283_bitmap), IPA_U0283_user_char);
    define_use_character_12_24_cmd(IPA_U028A_bitmap, sizeof(IPA_U028A_bitmap), IPA_U028A_user_char);
    define_use_character_12_24_cmd(IPA_U028C_bitmap, sizeof(IPA_U028C_bitmap), IPA_U028C_user_char);
    define_use_character_12_24_cmd(IPA_U0292_bitmap, sizeof(IPA_U0292_bitmap), IPA_U0292_user_char);
    define_use_character_12_24_cmd(IPA_U02A7_bitmap, sizeof(IPA_U02A7_bitmap), IPA_U02A7_user_char);
    define_use_character_12_24_cmd(IPA_U02D0_bitmap, sizeof(IPA_U02D0_bitmap), IPA_U02D0_user_char);
    define_use_character_12_24_cmd(IPA_U03B8_bitmap, sizeof(IPA_U03B8_bitmap), IPA_U03B8_user_char);

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
        else if (utf8_is_u_2byte(c))
        {
            int c2 = getchar();
            int u = utf8_get_u_2byte(c, c2);

            switch (u)
            {
            case 0x023E: /* Ⱦ = Test */
                //printf("Test");
                put_user_char(IPA_Utest_user_char);
                break;

            case 0x0E6: /* æ  */
                //printf("@");     break;
                put_user_char(IPA_U00E6_user_char);
                break;

            case 0x0F0: /* ð  */
                //printf("<Z>");   break;
                put_user_char(IPA_U00F0_user_char);
                break;

            case 0x14B: /* ŋ  */
                //printf("<n>");   break;
                put_user_char(IPA_U014B_user_char);
                break;

            case 0x251: /* ɑ  */
                //printf("a");     break;
                put_user_char(IPA_U0251_user_char);
                break;

            case 0x252: /* ɒ  */
                //printf("<o>");   break;
                put_user_char(IPA_U0252_user_char);
                break;

            case 0x254: /* ɔ  */
                //printf("o");     break;
                put_user_char(IPA_U0254_user_char);
                break;

            case 0x259: /* ə  */
                //printf("<a>");   break;
                put_user_char(IPA_U0259_user_char);
                break;

            case 0x25B: /* ɛ  */  
                //printf("e");     break;
                put_user_char(IPA_U025B_user_char);
                break;

            case 0x25C: /* ɜ  */  
                //printf("<e>");   break;
                put_user_char(IPA_U025C_user_char);
                break;

            case 0x26A: /* ɪ  */
                //printf("<i>");   break;
                put_user_char(IPA_U026A_user_char);
                break;

            case 0x283: /* ʃ  */
                //printf("<sh>");  break;
                put_user_char(IPA_U0283_user_char);
                break;

            case 0x28A: /* ʊ  */
                //printf("<u>");   break;
                put_user_char(IPA_U028A_user_char);
                break;

            case 0x28C: /* ʌ  */
                //printf("^");     break;
                put_user_char(IPA_U028C_user_char);
                break;

            case 0x292: /* ʒ  */
                //printf("<z>");   break;
                put_user_char(IPA_U0292_user_char);
                break;

            case 0x2A4: /* ʤ  */  
                //printf("<dz>");
                putchar('d');                        /* d */
                put_user_char(IPA_U0292_user_char);  /* ʒ */
                // skip a space symbol after dʒ.
                if ((c = getchar()) != ' ')
                    ungetc(c, stdin); 
                break;

            case 0x2A7: /* ʧ  */
                //printf("<tch>"); break;
                put_user_char(IPA_U02A7_user_char);
                break;

            case 0x2C8: /* ˈ  */  printf("'");     break;
            case 0x2CC: /* ˌ  */  printf(",");     break;

            case 0x2D0: /* ː  */
                //printf(":");     break;
                put_user_char(IPA_U02D0_user_char);
                break;

            case 0x3B8: /* θ  */
                //printf("<S>");   break;
                put_user_char(IPA_U03B8_user_char);
                break;

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

