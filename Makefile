CC = g++
FLAGS = -g -Wall
INCLUDE = -lSDL2main -lSDL2 -lSDL2_ttf
INCLUDEEXTLIB = -lSDL2_ttf
INCLUDELIB = $$(sdl2-config --libs)
INCLUDECFLAGS = $$(sdl2-config --cflags)
EXEC = bin/ilos
SRC = $(wildcard src/*.cpp)
HEAD = $(SRC:.cpp=.hpp)
OBJ = $(SRC:.cpp=.o)
RM = rm

all : $(EXEC)

%.o : %.cpp
	$(CC) $(FLAGS) -o $@ -c $< $(INCLUDECFLAGS) $(INCLUDEEXTLIB)
$(EXEC) : $(OBJ)
	$(CC) $(FLAGS) -o $@ $^ $(INCLUDELIB) $(INCLUDEEXTLIB)

.PHONY : clean

clean :
	$(RM) src/*.o

suppr :
	$(RM) src/*.o
	$(RM) bin/*

bin : $(EXEC)
	$(RM) src/*.o

# $@ nom cible
# $< nom premiere dependance
# $^ nom toutes les dependances
