all: oxml2osis

oxml2osis: oxml2osis.c isv_docx.h s_list.o libmxml.a
	gcc -Wall -pedantic -g -o $@ $< s_list.o libmxml.a -lpthread

s_list.o: s_list.c s_list.h
	gcc -Wall -pedantic -c -g -o $@ $<
