

CC=gcc
LIB_DIR=../lib/
#INC_DIR=./include/
#SRC_DIR=./src/

# 
# regras para compilacao e limpeza dos
# testes da libpithread
#

all: create wait mutex mutex-wait

create: create.c $(LIB_DIR)/libpithread.a
	$(CC) -o create create.c -L$(LIB_DIR) -lpithread -lm -Wall

yield: yield.c $(LIB_DIR)/libpithread.a
	$(CC) -o yield yield.c -L$(LIB_DIR) -lpithread -lm -Wall

wait: wait.c $(LIB_DIR)/libpithread.a
	$(CC) -o wait wait.c -L$(LIB_DIR) -lpithread -lm -Wall

mutex: mutex.c $(LIB_DIR)/libpithread.a
	$(CC) -o mutex mutex.c -L$(LIB_DIR) -lpithread -lm -Wall

mutex-wait: mutex-wait.c $(LIB_DIR)/libpithread.a
	$(CC) -o mutex-wait mutex-wait.c -L$(LIB_DIR) -lpithread -lm -Wall

clean:
	rm -rf create yield wait mutex pares mutex-wait *~
