default: sm2_test.exe

BITS=32
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

sm2_test.exe: sm2_test.c sm2.c sm2.h
	${COMPILER} ${CFLAG} -o sm2_test.exe sm2_test.c sm2.c

run: sm2_test.exe
	./sm2_test.exe

clean:
	rm -rf *.o *.exe

