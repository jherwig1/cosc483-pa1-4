all: driver problem2 problem3

driver: sha256.o driver.cpp
	g++ -o driver sha256.o driver.cpp -lssl -lcrypto
problem2: sha256.o
	g++ -o problem2 sha256.o problem2.cpp
problem3: sha256.o
	g++ -o problem3 sha256.o problem3.cpp
sha256.o: sha256.cpp sha256.h
	g++ -c sha256.h sha256.cpp

clean:
	rm problem2 problem3 driver sha256.o
