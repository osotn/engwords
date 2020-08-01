/*
 * English words
 * by Oleksandr Sotnikov
 * (c) 2017-2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define DEBUG_VERBOSITY 

#define MAX_WORD_NUM  100000
#define MAX_WORD_SIZE 100
#define MAX_PHONETIC_SIZE 100
#define MAX_TRANSLATION_SIZE 500
#define NUM_US_IN_MS 1000
#define PLAYER_NAME "mplayer"
#define PLAYER_OUTPUT ">/dev/null 2>&1"

#define SOUND_FILE_NAME "%ssound/%s.mp3"
#define SOUND_FILE_NAME_SIZEOF (3 + sizeof(SOUND_FILE_NAME) + MAX_WORD_SIZE)

#define PHONETIC_FILE_NAME "%sphonetics/%s"
#define PHONETIC_FILE_NAME_SIZEOF (3+ sizeof(PHONETIC_FILE_NAME) + MAX_WORD_SIZE)

#define TRANSLATION_FILE_NAME "%stranslations/%s"
#define TRANSLATION_FILE_NAME_SIZEOF (3 + sizeof(TRANSLATION_FILE_NAME) + MAX_WORD_SIZE)

#define PLAYER_CMD "mplayer ""\""SOUND_FILE_NAME"\""" >/dev/null 2>&1 </dev/null"
#define PLAYER_CMD_SIZEOF (sizeof(PLAYER_CMD) + MAX_WORD_SIZE)

#define FIRST_1000_FILE_NAME "%swords/first_1000/%s.txt"
#define FIRST_1000_FILE_NAME_SIZEOF (3 + sizeof(FIRST_1000_FILE_NAME) + MAX_WORD_SIZE)

#define OXFORD_3000_KEYS_FILE_NAME "%swords/oxford_3000_keys/%s.txt"
#define OXFORD_3000_KEYS_FILE_NAME_SIZEOF (3 + sizeof(OXFORD_3000_KEYS_FILE_NAME) + MAX_WORD_SIZE)

#define IELTS_4000_KEYS_FILE_NAME "%swords/ielts_general_4000/%s.txt"
#define IELTS_4000_KEYS_FILE_NAME_SIZEOF (3 + sizeof(IELTS_4000_KEYS_FILE_NAME) + MAX_WORD_SIZE)

#define TOEFL_5000_KEYS_FILE_NAME "%swords/toefl_5000/%s.txt"
#define TOEFL_5000_KEYS_FILE_NAME_SIZEOF (3 + sizeof(TOEFL_5000_KEYS_FILE_NAME) + MAX_WORD_SIZE)

#define FIRST_10000_FILE_NAME "%swords/first_10000/%s.txt"
#define FIRST_10000_FILE_NAME_SIZEOF (3 + sizeof(FIRST_10000_FILE_NAME) + MAX_WORD_SIZE)

#define MY_ACTIVE_FILE_NAME "%swords/active/%s"
#define MY_ACTIVE_FILE_NAME_SIZEOF (3 + sizeof(MY_ACTIVE_FILE_NAME) + MAX_WORD_SIZE) 

#ifndef BOOLEAN
#define BOOLEAN int
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


/* Flags */
static BOOLEAN fUnique = FALSE;
static BOOLEAN fNew = FALSE;
static BOOLEAN fNoDelay = FALSE;	/* -f: Fast mode */
static BOOLEAN fNoSound = FALSE;	/* -m: Mute mode */
static BOOLEAN fPutChars = FALSE;	/* -p: Putchar mode */
static BOOLEAN fWithoutColor = FALSE;	/* -b: Black mode */
/* Only for not unique mode */
static BOOLEAN fNoInfo = FALSE;		/* -c: No info mode */

static int delay_sound_word_ms = 1000;

static char *lang = "";                 /* english = "" */

static void get_opts(int argc, char *const argv[])
{
  int option;
  while ((option = getopt(argc, argv, "unfmgpcbd:")) != -1)
    switch (option)
    {
      case 'u':
        fUnique = TRUE;
        break;
      case 'n':
        fNew = TRUE;
        break;
      case 'f':
        fNoDelay = TRUE;
        break;
      case 'm':
        fNoSound = TRUE;
        break;
      case 'g':
	lang="de/"; /* germany */
        break; 

      case 'p':
        fPutChars = TRUE;
        break;

      case 'c':
        fNoInfo = TRUE;
        break;

      case 'b':
        fWithoutColor = TRUE;
        break;

      case 'd':
        delay_sound_word_ms = atoi(optarg);
        break;

      case '?':
        printf("Unknown option: %c\n", option);
        break;
    }
}

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
    if (fPutChars)
	putchar(c);

    if (c != ' ' && c != '\t' && c != '\n' && c != '\"' && c != ',' && c != '>')
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
  int i;

  if (words_index >= MAX_WORD_NUM)
    return -1;

  str_copy(words[words_index].word, s, MAX_WORD_NUM);
  words[i = words_index++].number = 1;    
  return i;
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
    i = add_new_word(s);
  else
    words[i].number++;

  return i;
}

int is_file(char *word, const char *file_format, char *fname)
{
   sprintf(fname, file_format, lang, word);
   //printf("File name %s", fname);
   return (access(fname, F_OK) != -1); 
}

int is_sound(char *s)
{
  char fname[SOUND_FILE_NAME_SIZEOF + 1];

  return is_file(s, SOUND_FILE_NAME, fname);
}

int is_phonetic(char *s)
{
  char fname[PHONETIC_FILE_NAME_SIZEOF + 1];

  return is_file(s, PHONETIC_FILE_NAME, fname);
}

int is_translation(char *s)
{
  char fname[TRANSLATION_FILE_NAME_SIZEOF + 1];

  return is_file(s, TRANSLATION_FILE_NAME, fname);
}

int is_first_1000(char *s)
{
  char fname[FIRST_1000_FILE_NAME_SIZEOF + 1];

  sprintf(fname, FIRST_1000_FILE_NAME, lang, s);
  return (access(fname, F_OK) != -1);
}

int is_oxford_3000_keys(char *s)
{
  char fname[OXFORD_3000_KEYS_FILE_NAME_SIZEOF + 1];

  sprintf(fname, OXFORD_3000_KEYS_FILE_NAME, lang, s);
  return (access(fname, F_OK) != -1);
}

int is_ielts_4000(char *s)
{
  char fname[IELTS_4000_KEYS_FILE_NAME_SIZEOF + 1];

  sprintf(fname, IELTS_4000_KEYS_FILE_NAME, lang, s);
  return (access(fname, F_OK) != -1);
}

int is_toefl_5000_keys(char *s)
{
  char fname[TOEFL_5000_KEYS_FILE_NAME_SIZEOF + 1];

  sprintf(fname, TOEFL_5000_KEYS_FILE_NAME, lang, s);
  return (access(fname, F_OK) != -1);
}

int is_first_10000(char *s)
{
  char fname[FIRST_10000_FILE_NAME_SIZEOF + 1];

  sprintf(fname, FIRST_10000_FILE_NAME, lang, s);
  return (access(fname, F_OK) != -1);
}

int is_my_active(char *s)
{
  char fname[MY_ACTIVE_FILE_NAME_SIZEOF + 1];

  sprintf(fname, MY_ACTIVE_FILE_NAME, lang, s);
  return (access(fname, F_OK) != -1);
}


int create_empty_file(char *s, int max_size)
{
  char cmd[MAX_WORD_SIZE + sizeof("touch .ext")];

  sprintf(cmd, "touch %s.txt", s);
  system(cmd);
  return TRUE;
}

void sound(char *s)
{
  char cmd[PLAYER_CMD_SIZEOF];

  sprintf(cmd, PLAYER_CMD, lang, s);
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

int get_file_line(const char *fname, char *line, int line_size)
{
  FILE *fp;

  line[0] = '\0';
  fp = fopen(fname, "r");
  if (fp == NULL)
    return -1;
  fgets(line, line_size, fp);
  fclose(fp);
  return 0;
} 

char *get_phonetic(char *s)
{
  static char phonetic[MAX_PHONETIC_SIZE + 1];
  char fname[PHONETIC_FILE_NAME_SIZEOF + 1];

  sprintf(fname, PHONETIC_FILE_NAME, lang, s);
  get_file_line(fname, phonetic, MAX_PHONETIC_SIZE+1);
  return phonetic;  
}

char *get_translation(char *s)
{
  static char translation[MAX_TRANSLATION_SIZE + 1];
  char fname[TRANSLATION_FILE_NAME_SIZEOF + 1];

  sprintf(fname, TRANSLATION_FILE_NAME, lang, s);
  get_file_line(fname, translation, MAX_TRANSLATION_SIZE+1);
  return translation;  
}

// ü = c3 bc; Ü = c3 9c
// ö = c3 b6; Ö = c3 96
// ä = c3 a4; Ä = c3 84
// ß = c3 9f; ẞ = e1 ba 9e  
int utf8_trick_isalpha(char *pc, int *pn)
{
  if (*(pc + 0) == (char)0xC3 &&
	  (
              *(pc + 1) == (char)0x9C || /* ü */
              *(pc + 1) == (char)0xBC || /* Ü */
              *(pc + 1) == (char)0x96 || /* ö */
              *(pc + 1) == (char)0xB6 || /* Ö */
              *(pc + 1) == (char)0x84 || /* ä */
              *(pc + 1) == (char)0xA4 || /* Ä */
              *(pc + 1) == (char)0x9F    /* ß */
	   )) {
    *pn = 2;
    return 1;         
  } else if ( *(pc + 0) == (char)0xE1 && *(pc + 1) == (char)0xBA && *(pc + 2) == (char)0x9E) {
    *pn = 3;
    return 1;
  } else {
    *pn = 1;
    return isalpha(*pc);
  }
}

int utf8_trick_tolower(char *pc, int *pn)
{
  char c;
  int  i = 0;
 
  *pn = 0;

  while ( (c = pc[i]) != '\0') {
  
    if (*(pc + i + 0) == (char)0xC3 &&
	  (
              *(pc + i + 1) == (char)0x9C || /* ü */
              *(pc + i + 1) == (char)0x96 || /* ö */
              *(pc + i + 1) == (char)0x84    /* ä */
	   )) {
        *(pc + i + 1) |= 0x20;
        i += 2;      
    } else if ( *(pc + i + 0) == (char)0xE1 && *(pc + i + 1) == (char)0xBA && *(pc + i + 2) == (char)0x9E) {
        *(pc + i + 0) = 0xC3;
	*(pc + i + 1) = 0x9F;
 	
	// shift s
	{
		int j=0;
		char *p = pc + i + 2;
		while ( (*p = *(p+1)) != '\0')
                   p++;
	}

	(*pn)--;
	i += 2;  
    } else {
        pc[i] = tolower(pc[i]);
	i++;
    }
  } 

}


int transform_word(char *s)
{
  char c;
  int i = 0, n;
 
  if (!utf8_trick_isalpha(s, &n))
    return 0;

  while ((c = s[i]) != '\0')
  {
    if (!utf8_trick_isalpha(s+i, &n)  && c != '\'' && c != '-')
    {
      s[i] = '\0';
      break;
    }
    
    i += n;
  }

  utf8_trick_tolower(s, &n);

  return i + n; 
}

#define TTY_LIGHT_CYAN "\033[0;36m"
#define TTY_NO_COLOR   "\033[0m"


void main(int argc, char * argv[])
{
  int i = 0;
  int len;
  int is_s, is_p, is_t, is_a, is_f1000, is_f3000, is_f4000, is_f5000, is_f10000;
  char *p;
  char word[MAX_WORD_SIZE + 1];
  int unknown_words = 0;
  int unsounded_words = 0;
  int untranslated_words = 0;

  get_opts(argc, argv);

  while ((len = get_word(word, MAX_WORD_SIZE, FALSE)) > 0)
  {
    int index;
    //printf(">%s\n", word);
    if (transform_word(word) == 0)
      continue;

    //printf(">>%s\n", word);
    index = add_word(word);

    if (!fUnique)
    {
      is_s = is_sound(word);
      is_p = is_phonetic(word);
      is_t = is_translation(word);
      is_a = is_my_active(word);
      is_f1000 = 0;//is_first_1000(word);
      is_f3000 = is_oxford_3000_keys/*is_first_3000*/(word);
      is_f4000 = is_ielts_4000(word);
      is_f5000 = is_toefl_5000_keys(word);
      is_f10000 = is_first_10000(word);
      p = is_f1000 ? "1T" : (is_f3000 ? "3T" : (is_f4000 ? "4T" : (is_f5000 ? "5T" : (is_f10000 ? "xT" : "xx"))));
      if (!fWithoutColor && !is_a)
        printf(TTY_LIGHT_CYAN);

      if (!fNoInfo)
      {
          printf("%5d,%5d(%3d). %s %s%s  %s", i, index, words[index].number, p,
              is_a ? "*" : "?", is_s ? "#" : " ", word);
          if (is_p)
              printf(" [%s]", get_phonetic(word));
          if (is_t)
              printf(" %s", get_translation(word));  
          printf("\n");
      }
      else
      {
          printf("%s ", word);
      }

      if (!fWithoutColor)
        printf(TTY_NO_COLOR);

      fflush(stdout);

      if (!fNoDelay) {
        if (is_s && !fNoSound) 
	  usleep(delay_sound_word_ms * NUM_US_IN_MS);
        else
          usleep((delay_sound_word_ms * 2 + 1) * NUM_US_IN_MS);
      }

      if (!fNoSound) {
          sound_if_possible(word);
      }
    }
    i++;
  }

  printf("\n");

#if 0
  if (argc > 1 && !strcmp(argv[1], "-f"))
  {
    i = 0;
    while (i < words_index)
    {
      if (!create_empty_file(words[i].word, MAX_WORD_SIZE))
        printf("Fail to create file for a word %s\n", words[i].word);
      i++;
    }
    printf("Created %d files\n", i);
  }
#endif

  if (!fUnique)
    return;

  printf("\n=== Word number is %d\n", i);

  printf("\n=== Unique word number is %d\n", words_index);

  for (i = 0; i < words_index; i++)
  {
    is_s = is_sound(words[i].word);
    is_p = is_phonetic(words[i].word);
    is_t = is_translation(words[i].word);
    is_a = is_my_active(words[i].word);

    if (!is_a)
      ++unknown_words;
    if (!is_s)
      ++unsounded_words;
    if (!is_t)
      ++untranslated_words;
 
    if (fNew && is_a)
      continue;

    is_f1000 = 0; //is_first_1000(words[i].word);
    is_f3000 = is_oxford_3000_keys/*is_first_3000*/(words[i].word);
    is_f4000 = is_ielts_4000(words[i].word);
    is_f5000 = is_toefl_5000_keys(words[i].word);
    is_f10000 = is_first_10000(words[i].word);
    p = is_f1000 ? "1T" : (is_f3000 ? "3T" : (is_f4000 ? "4T" : (is_f5000 ? "5T" : (is_f10000 ? "xT" : "xx"))));
//    printf("%s\n", words[i].word);
    if (!fWithoutColor && !is_a)
       printf(TTY_LIGHT_CYAN);
    printf("%5d. (%3d) %s %s%s%s%s %s", i, words[i].number,
           p, is_a ? "*" : "?", is_s ? "#" : " ",  is_p ? "P" : " ", is_t ? "T" : " ", words[i].word);
    if (is_p)
      printf(" [%s]", get_phonetic(words[i].word));
    if (is_t)
      printf(" %s", get_translation(words[i].word));  
    printf("\n");
    if (!fWithoutColor)
      printf(TTY_NO_COLOR);

    if (!fNoSound) {
      sound_if_possible(words[i].word);
      usleep(delay_sound_word_ms * NUM_US_IN_MS);
    }

    if (!fNoDelay) {
      if (!is_s)
        sleep(10);
    }

 }

  printf("\n=== Words: %d unknown, %d unsounded, %d untranslated\n",
      unknown_words, unsounded_words, untranslated_words);

}

/* EOF */

