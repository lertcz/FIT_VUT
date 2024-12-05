all: main

CC = gcc
override CFLAGS += -std=c11 -Wall -Wextra -Werror#-Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-result -Wno-unknown-pragmas -pedantic

.PHONY: run test test2

main: tnine.c
	$(CC) $(CFLAGS) tnine.c -o tnine
	chmod +x ./tnine

run: tnine
	./tnine < seznam.txt # jAnA NovotnA | bEdriCh SmetAnA ml. # -s 222

test: tnine
	./tnine     < seznam.txt
	./tnine 12  < seznam.txt
	./tnine 686 < seznam.txt
	./tnine 38  < seznam.txt
	./tnine 111 < seznam.txt

test2: tnine
	chmod +x ./tnine
	./tnine -s 3134      < seznam.txt # 60(31)2(34)56
	./tnine -s 77797654  < seznam.txt # (7779)8(7654)
	./tnine -s 11112     < seznam.txt # 54(11)4(112)0
	./tnine -s 73  < seznam.txt # petR Dvorak | bEDRich Smetana ml. 
	./tnine -s 222 < seznam.txt # jAnA NovotnA | bEdriCh SmetAnA ml.
	./tnine -s 247 < seznam.txt # bedriCH Smetana ml.