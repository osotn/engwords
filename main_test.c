#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>


int __wrap_getchar(void)
{
    return (int)mock();
}

#define BOOLEAN int
#define FALSE 0
#define TRUE 1
int get_word(char *s, int len_max, BOOLEAN b_print);


void test_get_word_hello (void ** state)
{
    char word[256] = {0};

    will_return(__wrap_getchar, (int)'h');
    will_return(__wrap_getchar, (int)'e');
    will_return(__wrap_getchar, (int)'l');
    will_return(__wrap_getchar, (int)'l');
    will_return(__wrap_getchar, (int)'o');
    will_return(__wrap_getchar, (int)' ');

    assert_int_equal(get_word(word, 256-1, FALSE), 5);
    assert_string_equal(word, "hello");
}

void test_get_word_max (void ** state)
{
    char word[256] = {0};

    will_return(__wrap_getchar, (int)'h');
    will_return(__wrap_getchar, (int)'e');
    will_return(__wrap_getchar, (int)'l');
    will_return(__wrap_getchar, (int)'l');
    will_return(__wrap_getchar, (int)'o');
    will_return(__wrap_getchar, (int)' ');

    assert_int_equal(get_word(word, 3, FALSE), 5);
    assert_string_equal(word, "hel");
}


int main (void)
{
    const struct CMUnitTest tests [] =
    {
        cmocka_unit_test (test_get_word_hello),
        cmocka_unit_test (test_get_word_max),
    };

    return cmocka_run_group_tests (tests, NULL, NULL);
}

