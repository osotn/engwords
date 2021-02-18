all: main.c utf8_ciril_to_oem866.c utf8_ipa_phonet_to_ascii.c
	gcc -o engwords main.c
	gcc -o utf8_ciril_to_oem866 utf8_ciril_to_oem866.c
	gcc -o utf8_ipa_phonet_to_ascii utf8_ipa_phonet_to_ascii.c



unittests:  all main_test.c
	gcc -o ut -DUNIT_TEST main.c main_test.c -Wl,--wrap=getchar -lcmocka
	./ut
