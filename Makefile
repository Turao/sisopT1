#
# Makefile ESQUELETO
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adqueuear este esqueleto de makefile para suas necessidades.
#
# OBSERVAR que as variáveis de ambiente consideram que o Makefile está no diretótio "pithread"
# 

CC=gcc
LIB_DIR=./lib/
INC_DIR=./include/
BIN_DIR=./bin/
SRC_DIR=./src/

# all: regra1 regra2 regran
all: libpithread scheduler list main binary

binary:
	$(CC) -o $(BIN_DIR)main $(BIN_DIR)main.o $(BIN_DIR)pithread.o $(BIN_DIR)list.o $(BIN_DIR)scheduler.o  -Wall
main:
	$(CC) -c $(SRC_DIR)main.c -I$(INC_DIR) -Wall
	mv main.o $(BIN_DIR)

libpithread:
	$(CC) -c $(SRC_DIR)pithread.c -I$(INC_DIR) -Wall
	mv pithread.o $(BIN_DIR)

list:
	$(CC) -c $(SRC_DIR)list.c -I$(INC_DIR) -Wall
	mv list.o $(BIN_DIR)

scheduler:
	$(CC) -c $(SRC_DIR)scheduler.c -I$(INC_DIR) -Wall
	mv scheduler.o $(BIN_DIR)

clean:
	rm -rf $(LIB_DIR)*.a $(BIN_DIR)*.o $(SRC_DIR)*~ $(INC_DIR)*~ *~

