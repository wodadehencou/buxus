default: buxus_test.exe

#CFLAG = -g3
CFLAG = -O3
#COMPILER = gcc
COMPILER = clang

bigint.o: bigint.c bigint.h buxus_type.h
	${COMPILER} ${CFLAG} -o bigint.o -c bigint.c

point.o: point.c point.h buxus_type.h
	${COMPILER} ${CFLAG} -o point.o -c point.c

sm2_algo.o: sm2_algo.c sm2_algo.h buxus_type.h
	${COMPILER} ${CFLAG} -o sm2_algo.o -c sm2_algo.c

buxus_test.o: buxus_test.c
	${COMPILER} ${CFLAG} -o buxus_test.o -c buxus_test.c

buxus_test.exe: bigint.o point.o sm2_algo.o buxus_test.o
	${COMPILER} ${CFLAG} -o buxus_test.exe bigint.o point.o sm2_algo.o buxus_test.o

run: buxus_test.exe
	./buxus_test.exe

clean:
	rm -rf *.o *.exe

help:
	@echo "Use \"make\" to do default compile (MAXIMUM performance)"
	@echo "Use \"make CFLAG=-O3\" to enable maximum optimization"
	@echo "Use \"make CFLAG=-g3\" to enable debug"
	@echo "Use \"make COMPILER=gcc\" to select gcc compiler"
	@echo "Use \"make run\" to run the test"

