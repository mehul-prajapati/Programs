CC = gcc
EXEC = myTest

SRC = $(shell find . -iname *.c)
OBJS = ${patsubst %.c, %.o, ${SRC}}

all:	${OBJS}
	@echo "============================"
	@echo "Buiding source files"
	@echo "============================="
	${CC} -o ${EXEC} $^

clean:
	rm -f ${EXEC} *.o

%.o: %.c
	${CC} -c $< -o $@


