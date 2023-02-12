CC=clang++
CFLAGS=-Wall -std=c++17 -O3 -DNDEBUG
EXEC_FILE=bootstrapping

all: $(EXEC_FILE)
default: $(EXEC_FILE)

$(EXEC_FILE): bootstrapping.cpp
	$(CC) $(CFLAGS) -o $(EXEC_FILE) bootstrapping.cpp

clean:
	rm -f $(EXEC_FILE)