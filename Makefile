all: main.c
	gcc -o engwords main.c

unittests:  all main_test.c
	gcc -o ut -DUNIT_TEST main.c main_test.c -Wl,--wrap=getchar -lcmocka
	./ut
