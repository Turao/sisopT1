#
# Makefile para programas exemplo
# 

CC=gcc
LIB_DIR=../lib

TESTS_SRC = tests/src/
TESTS_BIN = tests/bin/

all: exemplo mandel teste_vetor teste_prio

exemplo: exemplo.c $(LIB_DIR)/libpithread.a
	$(CC) -o exemplo exemplo.c -L$(LIB_DIR) -lpithread -Wall

teste_vetor: teste_vetor.c $(LIB_DIR)/libpithread.a
	$(CC) -o teste_vetor teste_vetor.c -L$(LIB_DIR) -lpithread -Wall

teste_prio: teste_prio.c $(LIB_DIR)/libpithread.a
	$(CC) -o teste_prio teste_prio.c -L$(LIB_DIR) -lpithread -Wall

mandel: mandel.c $(LIB_DIR)/libpithread.a
	$(CC) -o mandel mandel.c -L$(LIB_DIR) -lpithread -lm -Wall


# 
# regras para compilacao e limpeza dos
# testes da libpithread
#

tests: create yield wait mutex

create:
	$(CC) -o $(TESTS_BIN)create $(TESTS_SRC)create.c -L$(LIB_DIR) -lpithread -lm -Wall

yield:
	$(CC) -o $(TESTS_BIN)yield $(TESTS_SRC)yield.c -L$(LIB_DIR) -lpithread -lm -Wall

wait:
	$(CC) -o $(TESTS_BIN)wait $(TESTS_SRC)wait.c -L$(LIB_DIR) -lpithread -lm -Wall

mutex:
	$(CC) -o $(TESTS_BIN)mutex $(TESTS_SRC)mutex.c -L$(LIB_DIR) -lpithread -lm -Wall



clean:
	rm -rf exemplo mandel teste_vetor teste_prio *~




