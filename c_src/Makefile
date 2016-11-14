default: buxus_test.exe

BITS=64
PERF=1
DBG=0

#COMPILER = gcc
COMPILER = clang
CFLAG=

ifeq (${DBG},1)
  CFLAG += -g3
else
  CFLAG = -O3
endif

ifeq (${BITS},64)
  CFLAG += -DBIT64
  BIGINT=bigint_64.c
else
  CFLAG += -DBIT32
  BIGINT=bigint.c
endif

ifeq (${PERF},1)
  CFLAG += -DLINUX_PERFORMANCE
endif

$(patsubst %.c,%.o,$(wildcard *.c)): buxus_type.h

bigint.o: ${BIGINT} bigint.h
	${COMPILER} ${CFLAG} -o bigint.o -c ${BIGINT}

point.o: point.c point.h 
	${COMPILER} ${CFLAG} -o point.o -c point.c

sm2_algo.o: sm2_algo.c sm2_algo.h 
	${COMPILER} ${CFLAG} -o sm2_algo.o -c sm2_algo.c

buxus_test.o: buxus_test.c
	${COMPILER} ${CFLAG} -o buxus_test.o -c buxus_test.c

buxus_test.exe: bigint.o point.o sm2_algo.o buxus_test.o
	${COMPILER} ${CFLAG} -lpthread -o buxus_test.exe bigint.o point.o sm2_algo.o buxus_test.o

run: buxus_test.exe
	./buxus_test.exe

perf: ./buxus_test.exe
	perf record -g -e cycles ./buxus_test.exe
	perf report

simple_perf: ./buxus_test.exe
	perf stat ./buxus_test.exe

clean:
	rm -rf *.o *.exe perf*

help:
	@echo "Use \"make\" to do default compile (MAXIMUM performance with performace test)"
	@echo "Use \"make DBG=0\" to enable maximum optimization"
	@echo "Use \"make DBG=1\" to enable debug"
	@echo "Use \"make COMPILER=gcc\" to select gcc compiler"
	@echo "Use \"make BITS=64\" to select 64bit design"
	@echo "Use \"make BITS=32\" to select 32bit design"
	@echo "Use \"make PERF=1\" to enable performance test on Linux"
	@echo "Use \"make PERF=0\" to disable performance test"
	@echo "Use \"make run\" to run the test"

