CC=g++ -std=c++11
CFLAGS=-g -lSDL2 -lSDL2_image -lSDL2_ttf

TARGET = program.exe

all: clean $(TARGET)

$(TARGET): main.cpp
					 $(CC) $(CFLAGS) -o $(TARGET) main.cpp

clean:
					 $(RM) $(TARGET)
