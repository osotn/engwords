/*
 * English words
 * by Oleksandr Sotnikov
 * (c) 2017-2023
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

#define FIRST_20000_FILE_NAME "%swords/first_20000/%s.txt"
#define FIRST_20000_FILE_NAME_SIZEOF (3 + sizeof(FIRST_20000_FILE_NAME) + MAX_WORD_SIZE)

#define FIRST_30000_FILE_NAME "%swords/first_30000/%s.txt"
#define FIRST_30000_FILE_NAME_SIZEOF (3 + sizeof(FIRST_30000_FILE_NAME) + MAX_WORD_SIZE)


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
static BOOLEAN fNoIndexes = FALSE;      /* -i: No indexes mode */
/* Only for not unique mode */
static BOOLEAN fNoInfo = FALSE;		/* -c: No info mode */
static BOOLEAN fNoWordNum = FALSE;      /* -w: No word number info */ 
static int how_many_sound_a_word = 1;	/* -s 1 */ 

static int delay_sound_word_ms = 1000;

static char *lang = "";                 /* english = "" */

static void get_opts(int argc, char *const argv[])
{
  int option;
  while ((option = getopt(argc, argv, "unfmgreltpcbiwd:s:")) != -1)
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
        lang="de/"; /* German */
        break;
      case 'r':
        lang="fr/"; /* French */
        break;
      case 'e':
        lang="es/"; /* Spainish */
        break;
      case 'l':
        lang="la/"; /* Latin */
	break;
      case 't':
	lang="tr/"; /* Turkish */
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
      case 'i':
        fNoIndexes = TRUE;
        break;
      case 'w':
        fNoWordNum = TRUE;
        break;
      case 'd':
        delay_sound_word_ms = atoi(optarg);
        break;
      case 's':
        how_many_sound_a_word = atoi(optarg);
        break;
      case '?':
      default :
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

int is_space(char c)
{
  if (c == ' ' || c == '\t' || c == '\n' || c == '\"' || c == ',' || c == '>') 
    return 1;

  return 0;
}

int get_word(char *s, int len_max, BOOLEAN b_print)
{
  int c;
  int len = 0;

  while ((c = getchar()) != EOF)
  {
    if (b_print)
      putchar(c);

    if (is_space(c))
    {
      if (len <= 0)
        continue; /* skip firsts spaces */

      break; /* end of a word */
    }

    if (len < len_max)
      s[len] = c;

    len++;
  }

  if (len > 0)
      s[(len < len_max) ? len : len_max] = '\0';

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

int is_first_20000(char *s)
{
  char fname[FIRST_20000_FILE_NAME_SIZEOF + 1];

  sprintf(fname, FIRST_20000_FILE_NAME, lang, s);
  return (access(fname, F_OK) != -1);
}

int is_first_30000(char *s)
{
   char fname[FIRST_30000_FILE_NAME_SIZEOF + 1];
 
   sprintf(fname, FIRST_30000_FILE_NAME, lang, s);
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

// German letters
// Ü = c3 9c; ü = c3 bc
// Ö = c3 96; ö = c3 b6
// Ä = c3 84; ä = c3 a4
// ẞ = e1 ba 9e; ß = c3 9f
// French letters
// À = c3 80; à = c3 a0
// Â = c3 82; â = c3 a2
// Æ = c3 86; æ = c3 a6
// Ç = c3 87; ç = c3 a7
// È = c3 88; è = c3 a8
// É = c3 89; é = c3 a9
// Ê = c3 8a; ê = c3 aa
// Ë = c3 8b; ë = c3 ab
// Î = c3 8e; î = c3 ae
// Ï = c3 8f; ï = c3 af
// Ô = c3 94; ô = c3 b4
// Ù = c3 99; ù = c3 b9
// Û = c3 9b; û = c3 bb
// Œ = c5 92; œ = c5 93
// Ÿ = c5 b8; ÿ = c3 bf
// Spain letters
// Ñ = c3 91; ñ = c3 b1
// Í = c3 8d; í = c3 ad
// Ó = c3 93; ó = c3 b3
// Ú = c3 9a; ú = c3 ba
// Á = c3 81; á = c3 a1
// Turkish
// Ğ = c4 9e; ğ = c4 9f
// Ş = c5 9e; ş = c5 9f
// İ = c4 b0; ı = c4 b1
int utf8_trick_isalpha(char *pc, int *pn)
{
  if (*(pc + 0) == (char)0xC3 &&
	  (
              *(pc + 1) == (char)0x9C || /* Ü */ *(pc + 1) == (char)0xBC || /* ü */
              *(pc + 1) == (char)0x96 || /* Ö */ *(pc + 1) == (char)0xB6 || /* ö */
              *(pc + 1) == (char)0x84 || /* Ä */ *(pc + 1) == (char)0xA4 || /* ä */
                                                 *(pc + 1) == (char)0x9F || /* ß */
              *(pc + 1) == (char)0x80 || /* À */ *(pc + 1) == (char)0xa0 || /* à */
              *(pc + 1) == (char)0x82 || /* Â */ *(pc + 1) == (char)0xa2 || /* â */
              *(pc + 1) == (char)0x86 || /* Æ */ *(pc + 1) == (char)0xa6 || /* æ */
              *(pc + 1) == (char)0x87 || /* Ç */ *(pc + 1) == (char)0xa7 || /* ç */
              *(pc + 1) == (char)0x88 || /* È */ *(pc + 1) == (char)0xa8 || /* è */
              *(pc + 1) == (char)0x89 || /* É */ *(pc + 1) == (char)0xa9 || /* é */
              *(pc + 1) == (char)0x8a || /* Ê */ *(pc + 1) == (char)0xaa || /* ê */
              *(pc + 1) == (char)0x8b || /* Ë */ *(pc + 1) == (char)0xab || /* ë */
              *(pc + 1) == (char)0x8e || /* Î */ *(pc + 1) == (char)0xae || /* î */
              *(pc + 1) == (char)0x8f || /* Ï */ *(pc + 1) == (char)0xaf || /* ï */
              *(pc + 1) == (char)0x94 || /* Ô */ *(pc + 1) == (char)0xb4 || /* ô */
              *(pc + 1) == (char)0x99 || /* Ù */ *(pc + 1) == (char)0xb9 || /* ù */
                                                 *(pc + 1) == (char)0xbf || /* ÿ */
              *(pc + 1) == (char)0x9b || /* Û */ *(pc + 1) == (char)0xbb || /* û */
              *(pc + 1) == (char)0x91 || /* Ñ */ *(pc + 1) == (char)0xb1 || /* ñ */
              *(pc + 1) == (char)0x81 || /* Á */ *(pc + 1) == (char)0xa1 || /* á */
              *(pc + 1) == (char)0x8d || /* Í */ *(pc + 1) == (char)0xad || /* í */
              *(pc + 1) == (char)0x93 || /* Ó */ *(pc + 1) == (char)0xb3 || /* ó */
              *(pc + 1) == (char)0x9a || /* Ú */ *(pc + 1) == (char)0xba    /* ú */
	   )) {
    *pn = 2;
    return 1;
  } else   if (*(pc + 0) == (char)0xC4 &&
     (
              *(pc + 1) == (char)0x9e || /* Ğ */ *(pc + 1) == (char)0x9f || /* ğ */
              *(pc + 1) == (char)0xb0 || /* İ */ *(pc + 1) == (char)0xb1    /* ı */
      )) {
    *pn = 2;
    return 1; 
  } else   if (*(pc + 0) == (char)0xC5 &&
      (
              *(pc + 1) == (char)0x92 || /* Œ */ *(pc + 1) == (char)0x93 || /* œ */
              *(pc + 1) == (char)0xb8 || /* Ÿ */
	      *(pc + 1) == (char)0x9e || /* Ş */ *(pc + 1) == (char)0x9f    /* ş */
      )) {
    *pn = 2;
    return 1;
  } else if ( *(pc + 0) == (char)0xE1 && *(pc + 1) == (char)0xBA && *(pc + 2) == (char)0x9E) /* ẞ */ {
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
                (((unsigned char)*(pc + i + 1) >= (unsigned char)0x80 &&
                  (unsigned char)*(pc + i + 1) <= (unsigned char)0x96)    ||
                 ((unsigned char)*(pc + i + 1) >= (unsigned char)0x98 &&
                  (unsigned char)*(pc + i + 1) <= (unsigned char)0x9E))) {
            *(pc + i + 1) |= 0x20;
            i += 2;
	} else if ((*(pc + i + 0) == (char)0xC4) && (*(pc + i + 1) == (char)0x9E)) { /* Ğ -> ğ */
	    *(pc + i + 1) = 0x9F;
	    i += 2;
	} else if ((*(pc + i + 0) == (char)0xc4) && (*(pc + i + 1) == (char)0xB0)) { /* İ -> ı */
	    *(pc + i + 1) = 0xB1;
	    i += 2;
        } else if ((*(pc + i + 0) == (char)0xC5) && (*(pc + i + 1) == (char)0x92)) { /* Œ -> œ */
            *(pc + i + 1) = 0x93;
            i += 2;
	} else if ((*(pc + i + 0) == (char)0xC5) && (*(pc + i + 1) == (char)0x9E)) { /* Ş -> ş */
	    *(pc + i + 1) = 0x9F;
	    i += 2;
        } else if ((*(pc + i + 0) == (char)0xC5) && (*(pc + i + 1) == (char)0xB8)) { /* Ÿ -> ÿ */
            *(pc + i + 0) = 0xC3;
            *(pc + i + 1) = 0xBf;
            i += 2;
        } else if ( *(pc + i + 0) == (char)0xE1 && *(pc + i + 1) == (char)0xBA && *(pc + i + 2) == (char)0x9E) {
            *(pc + i + 0) = 0xC3;
            *(pc + i + 1) = 0x9F;                   /* ẞ -> ß */

            // shift string on one char right.
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
    } /* while */
}

/* May skip the first characters of a word */
int transform_word(char **p_s)
{
  char c;
  int i = 0, n;

  /* skip the first '(' */
  while (**p_s == '(')
      ++(*p_s);

  if (!utf8_trick_isalpha(*p_s, &n))
    return 0;

  while ((c = (*p_s)[i]) != '\0')
  {
    if (!utf8_trick_isalpha(*p_s + i, &n)  && c != '\'' && c != '-' && c != '+')
    {
      (*p_s)[i] = '\0';
      break;
    }
    
    i += n;
  }

  utf8_trick_tolower(*p_s, &n);

  return i + n; 
}

#define TTY_LIGHT_CYAN "\033[0;36m"
#define TTY_NO_COLOR   "\033[0m"


#ifdef UNIT_TEST
  #define main main_app
#endif
void main(int argc, char * argv[])
{
  int i = 0;
  int len;
  int is_s, is_p, is_t, is_a, is_f1000, is_f3000, is_f4000, is_f5000, is_f10000, is_f20000, is_f30000;
  char *p;
  char word[MAX_WORD_SIZE + 1];
  int unknown_words = 0;
  int unsounded_words = 0;
  int untranslated_words = 0;

  get_opts(argc, argv);

  while ((len = get_word(word, MAX_WORD_SIZE, fPutChars)) > 0)
  {
    int index;
    char * p_word = word;
    //printf(">%s\n", word);
    if (transform_word(&p_word) == 0)
      continue;

    //printf(">>%s\n", word);
    index = add_word(p_word);

    if (!fUnique)
    {
      is_s = is_sound(p_word);
      is_p = is_phonetic(p_word);
      is_t = is_translation(p_word);
      is_a = is_my_active(p_word);
      is_f1000 = is_first_1000(p_word);
      is_f3000 = is_oxford_3000_keys/*is_first_3000*/(p_word);
      is_f4000 = is_ielts_4000(p_word);
      is_f5000 = is_toefl_5000_keys(p_word);
      is_f10000 = is_first_10000(p_word);
      is_f20000 = is_first_20000(p_word);
      is_f30000 = is_first_30000(p_word);
      p = is_f1000 ? "1T" : (is_f3000 ? "3T" : (is_f4000 ? "4T" : (is_f5000 ? "5T" : (is_f10000 ? "xT" : (is_f20000 ? "yT" : (is_f30000 ? "zT" : "xx"))))));
      if (!fWithoutColor && !is_a)
        printf(TTY_LIGHT_CYAN);

      if (!fNoInfo)
      {
          if (!fNoIndexes)
              printf("%5d,%5d(%3d). ", i, index, words[index].number);
          printf("%s %s%s  %s", p, is_a ? "*" : "?", is_s ? "#" : " ", p_word);
          if (is_p)
              printf(" [%s]", get_phonetic(p_word));
          if (is_t)
              printf(" %s", get_translation(p_word));  
          printf("\n");
      }
      else
      {
          printf("%s ", p_word);
      }

      if (!fWithoutColor)
        printf(TTY_NO_COLOR);

      fflush(stdout);

      { int i;
      for (i = 0; i < how_many_sound_a_word; i++)
      {
          if (!fNoDelay) {
              if (is_s && !fNoSound) 
	          usleep(delay_sound_word_ms * NUM_US_IN_MS);
          else
              usleep((delay_sound_word_ms * 2 + 1) * NUM_US_IN_MS);
          }

          if (!fNoSound) {
              sound_if_possible(p_word);
          }
      }
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

  if (!fNoWordNum)
      printf("\n=== Word number is %d\n", i);

  if (!fNoWordNum)
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
    is_f20000 = is_first_20000(words[i].word);
    is_f30000 = is_first_30000(words[i].word);
    p = is_f1000 ? "1T" : (is_f3000 ? "3T" : (is_f4000 ? "4T" : (is_f5000 ? "5T" : (is_f10000 ? "xT" : (is_f20000 ? "yT" : (is_f30000 ? "zT" : "xx"))))));
//    printf("%s\n", words[i].word);
    if (!fWithoutColor && !is_a)
       printf(TTY_LIGHT_CYAN);
    if (!fNoIndexes)
       printf("%5d. (%3d) ", i, words[i].number); 
    printf("%s %s%s%s%s %s", p, is_a ? "*" : "?", is_s ? "#" : " ",  is_p ? "P" : " ", is_t ? "T" : " ", words[i].word);
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

  if (!fNoWordNum) {
      printf("\n=== Words: %d unknown, %d unsounded, %d untranslated\n",
          unknown_words, unsounded_words, untranslated_words);
  }
}

/* EOF */

