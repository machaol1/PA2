CXX			= g++
CXXFLAGS	= -std=c++17 -pedantic -Wall -o2
CFLAGS		= -Wall -pedantic
# -fsanitize=address
LIBS		= -lncurses -lstdc++fs

# class files names
ALL	= Enemy Bomb Buff Game Map Movable Object Player Wall Menu main

# adds .o suffix to all (main -> main.o)
MODULES	= $(addsuffix .o, $(ALL))

compile: machaol1

machaol1: $(MODULES)
	$(CXX) $(CXXFLAGS) $(MODULES) -o machaol1 $(LIBS)

# compile a C++ module
%.o : src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run:
	./machaol1

doc:
	doxygen dconfig > /dev/null

clean:
	rm -f $(MODULES) machaol1 && rm -r doc

l:
	wc -l src/*.cpp #src/*.h

# here add output of "g++ -MM src/*.cpp >>Makefile"
Bomb.o: src/Bomb.cpp src/Bomb.h src/Object.h
Buff.o: src/Buff.cpp src/Buff.h src/Object.h
Enemy.o: src/Enemy.cpp src/Enemy.h src/Movable.h src/Object.h src/Player.h src/Buff.h
Game.o: src/Game.cpp src/Game.h src/Player.h src/Movable.h src/Object.h src/Buff.h src/Map.h src/Enemy.h src/Bomb.h src/Menu.h
Map.o: src/Map.cpp src/Map.h src/Player.h src/Movable.h src/Object.h src/Buff.h src/Enemy.h src/Bomb.h src/Wall.h
Menu.o: src/Menu.cpp src/Menu.h
Movable.o: src/Movable.cpp src/Movable.h src/Object.h
Object.o: src/Object.cpp src/Object.h
Player.o: src/Player.cpp src/Player.h src/Movable.h src/Object.h src/Buff.h src/Bomb.h src/Game.h src/Map.h src/Enemy.h
Wall.o: src/Wall.cpp src/Wall.h src/Object.h
main.o: src/main.cpp src/Game.h src/Player.h src/Movable.h src/Object.h src/Buff.h src/Map.h src/Enemy.h src/Bomb.h
