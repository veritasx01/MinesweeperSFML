CXX = g++
CXXFLAGS = -std=c++17 -I/ucrt64/include -L/ucrt64/lib

all: a

a: main.o mines_logic.o
	$(CXX) $(CXXFLAGS) main.cpp -o minesweeper -lsfml-graphics -lsfml-window -lsfml-system
	rm -f *.o a

clean:
	rm -f *.o a

