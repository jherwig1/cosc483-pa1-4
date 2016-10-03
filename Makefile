all: driver

driver: sha256.o driver.cpp
	g++ -o driver sha256.o driver.cpp -lssl -lcrypto
sha256.o: sha256.cpp sha256.h
	g++ -c sha256.h sha256.cpp
