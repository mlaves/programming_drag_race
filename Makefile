CC=clang++
CFLAGS=-Wall -std=c++17 -O3 -DNDEBUG
INCL=-I./eigen
EXEC_FILE=bootstrapping_cpp

all: $(EXEC_FILE)
default: $(EXEC_FILE)

eigen:
	git clone https://gitlab.com/libeigen/eigen.git $@

$(EXEC_FILE): bootstrapping.cpp | eigen
	$(CC) $(CFLAGS) $(INCL) -o $(EXEC_FILE) bootstrapping.cpp

clean:
	rm -f $(EXEC_FILE)