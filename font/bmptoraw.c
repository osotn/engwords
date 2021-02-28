#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<sys/time.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

/* bmptoraw: utility to convert bitmap files to raw 16 bit data
   
   Usage:
   ./bmptoraw pic.bmp > pic.raw
   
   pic.raw can then be used for a splashscreen on the TS-TPC-7390:
   
   cat pic.raw > /dev/fb0
   
   This program assumes a 24 bit bitmap file with a 54 byte header,
   which is the most common form of bitmap.  One can convert
   other image files to that format using GIMP or MS Paint.
   
   Compiled on the ARM platform:
   gcc -Wall -mcpu=arm9 bmptoraw.c -o bmptoraw
*/

int main(int argc, char **argv) {

  volatile unsigned char *bmp;
  int fd;
  int i, j, offset;
  int length, page;
  char red, blue, green, char1, char2;
  int w, h;
  int b;
  unsigned char hex[12*16] = {0};

  assert(argc = 2);
  
  page = getpagesize();
  length = (((480*800*4 + 138)/page) + 1) * page;
  
  fd = open(argv[1], O_RDONLY);
  bmp = (unsigned char *)mmap(0, length,
    PROT_READ, MAP_SHARED, fd, 0);
  
  assert(bmp[0] == 'B');
  assert(bmp[1] == 'M');
  
  assert(bmp[0xa] == 54);
  
  w = bmp[0x12];
  h = bmp[0x16];

  assert(w == 12);
  assert(h == 24);

  printf("w = %d. h =%d\n", w, h);
  printf("/*\n");
  for(i=0;i<h;i++) {
    if (i!=0) putchar('\n');
    for(j=0;j<w;j++) {
      offset =  54 + j * 3 + w*(h-1-i)*3;
      blue = bmp[offset];
      green = bmp[offset + 1];
      red = bmp[offset + 2];
      char2 = red & 0xf8;
      char2 |= green >> 5;
      char1 = blue >> 3;
      char1 |= (green & 0xf8) << 3;
      if ((unsigned)bmp[offset] < (unsigned)0xFF)
          putchar('*');
      else
          putchar(' ');
      //printf("%02x", bmp[offset]);
      //putchar(char1);
      //putchar(char2);
    }
  }
  printf("\n*/\n");
  b = 0;
  for(j=0;j<w;j++) {
    for(i=0;i<h;i++) { 

      offset =  54 + j * 3 + w*(h-1-i)*3;
      if ((unsigned)bmp[offset] < (unsigned)0xFF) {
          //putchar('1');
          hex[b/8] |= 1u << (7 - b%8);
      }
      else {
          //putchar('0');
      }
      //printf("%02x", bmp[offset]);
      //putchar(char1);
      //putchar(char2);

      if ((++b % 8) == 0) {
          //printf("\'\\x%.2X\',\n", hex[(b-1)/8]);
      }
    }
  }

  b = 0;
  for(j=0;j<w;j++) {
    for(i=0;i<h;i++) { 

      if ((++b % 24) == 0) {
          printf("        \'\\x%.2X\', \'\\x%.2X\', \'\\x%.2X\',\n",
                 hex[(b-1)/8 -2], hex[(b-1)/8 -1], hex[(b-1)/8 - 0]);
      }

    }
  }

  munmap((unsigned int *)bmp, length);
  close(fd);
  return 0;
}
