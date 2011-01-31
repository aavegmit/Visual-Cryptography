# Macros

CC = g++
CFLAGS = -g -Wall
OBJ = main.o stream.o encrypt.o decrypt.o 
LIBS = -lcrypto
#LIBS = -L/home.scf-22/csci551b/openssl/lib -lcrypto -lnsl -lsocket -lresolv
#INC = -I/home/scf-22/csci551b/openssl/include
INC = 

# Explicit rule
all: hw2 

hw2: $(OBJ)
	$(CC) $(CFLAGS) -o hw2 $(OBJ) $(INC) $(LIBS) 

clean:
	rm -rf *.o hw2

main.o: main.cc
	$(CC) $(CFLAGS) -c main.cc $(INC)

stream.o: stream.cc
	$(CC) $(CFLAGS) -c stream.cc $(INC)
encrypt.o: encrypt.cc
	$(CC) $(CFLAGS) -c encrypt.cc $(INC)
decrypt.o: decrypt.cc
	$(CC) $(CFLAGS) -c decrypt.cc $(INC)
