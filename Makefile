MAIN :=	src/main.c
BINARY_NAME := bin/server

.PHONY: run
run:
	gcc ${MAIN} -o ${BINARY_NAME}
	./bin/server