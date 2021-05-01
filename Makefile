CC = g++

CFLAGS = -Ofast -Wall -Werror -std=c++11

TARGET = coinminer

LIB = libssl-dev

packages: 
	sudo apt-get install libssl-dev
coin: $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp -lssl -lcrypto