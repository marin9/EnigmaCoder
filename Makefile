CC=g++
INPUT=src/main.cpp src/lcd.cpp src/keyboard.cpp src/gtext.cpp src/enigma.cpp src/plugboard.cpp
CFLAGS=-Wall -o 
LIBS=-lSDL2
TARGET=EnigmaMachine

$(TARGET): $(INPUT)
	$(CC) $(CFLAGS) $(TARGET) $(INPUT) $(LIBS) 
