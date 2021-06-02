all: main.c utf8_ciril_to_oem866.c utf8_to_pos.c
	gcc -o engwords main.c
	gcc -o utf8_to_pos utf8_to_pos.c

unittests:  all main_test.c
	gcc -o ut -DUNIT_TEST main.c main_test.c -Wl,--wrap=getchar -fno-builtin -lcmocka
	./ut

clean:
	rm -f main.o utf8_to_pos.o main_test.o engwords utf8_to_pos ut 

