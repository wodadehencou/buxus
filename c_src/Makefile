default: buxus_test.exe

bigint.o: bigint.c bigint.h buxus_type.h
	gcc -g3 -o bigint.o -c bigint.c

point.o: point.c point.h buxus_type.h
	gcc -g3 -o point.o -c point.c

sm2_algo.o: sm2_algo.c sm2_algo.h buxus_type.h
	gcc -g3 -o sm2_algo.o -c sm2_algo.c

buxus_test.o: buxus_test.c
	gcc -g3 -o buxus_test.o -c buxus_test.c

buxus_test.exe: bigint.o point.o sm2_algo.o buxus_test.o
	gcc -g3 -o buxus_test.exe bigint.o point.o sm2_algo.o buxus_test.o

run: buxus_test.exe
	./buxus_test.exe

clean:
	rm -rf *.o *.exe

