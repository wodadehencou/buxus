
default: run
	
run: sm2test.exe
	./sm2test.exe

export LD_LIBRARY_PATH=.

#LIBS= -lssl -lcrypto -L. 
#LIBS= ./libssl.so ./libcrypto.so
LIBS= \
	 ./libssl.a \
	 ./libcrypto.a 

sm2test.o: ./sm2test.c
	gcc -pg -I./include -Wall -g3 -c -o sm2test.o ./sm2test.c

sm2test.exe: sm2test.o
	gcc -pg -g3 sm2test.o ${LIBS} -o sm2test.exe

clean:
	$(RM) sm2test.o sm2test.exe



