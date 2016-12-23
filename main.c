#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DEBUG_VERBOSITY 

#define MAX_WORD_NUM  1000
#define MAX_WORD_SIZE 100
#define PLAYER_NAME "mplayer"
#define PLAYER_OUTPUT ">/dev/null 2>&1"

#define SOUND_FILE_NAME "sound/%s.mp3"
#define SOUND_FILE_NAME_SIZEOF (sizeof(SOUND_FILE_NAME) + MAX_WORD_SIZE)

#define PLAYER_CMD "mplayer ""\""SOUND_FILE_NAME"\""" >/dev/null 2>&1 </dev/null"
#define PLAYER_CMD_SIZEOF (sizeof(PLAYER_CMD) + MAX_WORD_SIZE)

#ifndef BOOLEAN
#define BOOLEAN int
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

static struct
{
  char word[MAX_WORD_SIZE + 1];
  int  number;  
} words[MAX_WORD_NUM]; 

static int words_index = 0;

int get_word(char *s, int len_max, BOOLEAN b_print)
{
  int c;
  int len = 0;

  while ((c = getchar()) != EOF)
  {
    putchar(c);

    if (c != ' ' && c != '\t' && c != '\n')
    {
      if (len < len_max)
        s[len] = c;

      len++;
      continue;
    }
    
    if (len > 0)
    {
      s[(len < len_max) ? len : len_max] = '\0';
      break;	
    }
  }

  return len;
}

int str_copy(char *d, char *s, int max_len)
{
  int i = 0;

  while (i++ < max_len && (*d++ = *s++) != '\0')
    ;

  if (i == max_len)
    d[i] = '\0';
  
  return i; 
}

int is_str_equal(char *s1, char *s2, int max_len)
{
  char c, l;
  int i = 0;

  while (i++ < max_len && (c = *s1++) == (l = *s2++) && c != '\0')
    ;

  if (c == '\0' && l == '\0')
    return 1;

  return 0; 
}

int add_new_word(char *s)
{
  if (words_index >= MAX_WORD_NUM)
    return -1;

  str_copy(words[words_index].word, s, MAX_WORD_NUM);
  words[words_index++].number = 1;    
  return 1;
}

int get_word_index(char *s)
{
  int i;

  for (i = 0; i < words_index; i++)
  {
    if (is_str_equal(s, words[i].word, MAX_WORD_SIZE) && words[i].number > 0)
      return i;   
  }

  return -1;
}

int add_word(char *s)
{
  int i;

  if ((i = get_word_index(s)) < 0)
    add_new_word(s);
  else
    words[i].number++;
}

int is_sound(char *s)
{
  char fname[SOUND_FILE_NAME_SIZEOF];

  sprintf(fname, SOUND_FILE_NAME, s);
  //printf("File name %s", fname);
  return (access(fname, F_OK) != -1); 
}

void sound(char *s)
{
  char cmd[PLAYER_CMD_SIZEOF];

  sprintf(cmd, PLAYER_CMD, s);
  //printf("File cmd %s sizeof = %d", cmd, (int)PLAYER_CMD_SIZEOF);
  system(cmd);
}

int sound_if_possible(char *s)
{
  if (!is_sound(s))
    return -1;

  sound(s);
  return 0;
}

int transform_word(char *s)
{
  char c;
  int i = 0;

 
  if (!isalpha(*s))
    return 0;

  while ((c = s[i]) != '\0')
  {
    if (!isalpha(c) && c != '\'' && c != '-')
    {
      s[i] = '\0';
      break;
    }
    
    s[i++] = tolower(c);
  }

  return i; 
}

void main(int argc, char * argv[])
{
  int i = 0;
  int len;
  int is_s;
  char word[MAX_WORD_SIZE + 1];

  while ((len = get_word(word, MAX_WORD_SIZE, FALSE)) > 0)
  {
    if (transform_word(word) == 0)
      continue;
 
    add_word(word);
  

    if (argc > 1 && (i >= atoi(argv[1])))
    {
      is_s = is_sound(word);
      printf("%5d. %s  %s\n", i, is_s ? "#" : " ", word);
      if (is_s)
        sleep(1);
      else
        sleep(2);
      sound_if_possible(word);    
    }
    i++;
  }

  if (argc > 1)
    return;

  printf("\n=== Word number is %d\n", i);

  i = 0;
  while (i < words_index)
  {
    is_s = is_sound(words[i].word);
    printf("%5d. (%3d) %s %s\n", i, words[i].number, is_s ? "#" : " ", words[i].word);
    sound_if_possible(words[i].word);    

    if (!is_s)
      sleep(10);

    i++; 
  }

  printf("\n=== Unique word number is %d\n", words_index);
}
