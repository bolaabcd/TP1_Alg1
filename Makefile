CC = g++
FLAGS = -std=c++17 -Wall -O2 #-O3
DBGFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer -g -Wall -Wshadow -std=c++17 -Wno-unused-result -Wno-sign-compare -Wno-char-subscripts

# Arquivos
TARGET = tp01

build: $(TARGET)

$(TARGET): TP1.cpp
	$(CC) $(FLAGS) -o $(TARGET) TP1.cpp

clean:
	rm $(TARGET)
