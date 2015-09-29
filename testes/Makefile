

CC=gcc
LIB_DIR=../lib/
#INC_DIR=./include/
#SRC_DIR=./src/

# 
# regras para compilacao e limpeza dos
# testes da libpithread
#

all: create wait mutex mutex-wait
# yield mutex

# tests: create yield wait mutex

create:
	$(CC) -o create create.c -L$(LIB_DIR) -lpithread -lm -Wall

yield:
	$(CC) -o yield yield.c -L$(LIB_DIR) -lpithread -lm -Wall

wait:
	$(CC) -o wait wait.c -L$(LIB_DIR) -lpithread -lm -Wall

mutex:
	$(CC) -o mutex mutex.c -L$(LIB_DIR) -lpithread -lm -Wall

mutex-wait:
	$(CC) -o mutex-wait mutex-wait.c -L$(LIB_DIR) -lpithread -lm -Wall

clean:
	rm -rf create yield wait mutex pares mutex-wait *~
