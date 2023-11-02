CC = g++
FLAGS = -g -Wall
INCLUDEWIN = -lSDL2main -lSDL2 -lSDL2_ttf
INCLUDEEXTLIB = -lSDL2_ttf
INCLUDELIB = $$(sdl2-config --libs)
INCLUDECFLAGS = $$(sdl2-config --cflags)
EXEC = ilos
SRC = $(wildcard $(SRCDIR)*.cpp)
HEAD = $(SRC:.cpp=.hpp)
OBJ = $(subst $(SRCDIR), $(OBJDIR), $(SRC:.cpp=.o))
VERSION = 0.5


RM = rm
DATE = $(shell date +%Y-%m-%d-%T)
SRCDIR = src/
BINDIR = bin/
DOCDIR = doc/
SAVEDIR = ../ilos_backup/
OBJDIR = obj/
DATADIR = data/
RESSOURCESDIR = ressources/
RELEASEDIR = ../ilos_releases/
NEWRELEASEDIR = ilos_v$(VERSION)_ubuntu/

.PHONY : clean
.PHONY : doc
.PHONY : bin
.PHONY : suppr
.PHONY : save
.PHONY : exec
.PHONY : give
.PHONY : obj
.PHONY : restore
.PHONY : release

all : $(BINDIR)$(EXEC)


$(OBJDIR)%.o : $(SRCDIR)%.cpp
	$(CC) $(FLAGS) -o $@ -c $< $(INCLUDECFLAGS) $(INCLUDEEXTLIB)

$(BINDIR)$(EXEC) : $(OBJ)
	$(CC) $(FLAGS) -o $@ $^ $(INCLUDELIB) $(INCLUDEEXTLIB)


clean :
	$(RM) -f $(OBJDIR)*.o
	$(RM) -f *.bak
	$(RM) -f *.old
	$(RM) -f *.*~

suppr :
	$(RM) -f $(OBJDIR)*.o
	$(RM) -f $(BINDIR)ilos*

bin : $(BINDIR)$(EXEC)
	$(RM) -f $(OBJDIR)*.o

exec : $(BINDIR)$(EXEC)
	./$(BINDIR)$(EXEC)

doc :
	mkdir -p $(DOCDIR)
	doxygen Doxyfile
	cp -r $(DOCDIR)* ~/docilo/

give :
	mkdir -p $(SAVEDIR)
	zip -r $(SAVEDIR)"$(DATE).zip" $(SRCDIR)* $(DATADIR)* $(DOCDIR)* $(RESSOURCESDIR)* Makefile Doxyfile README.md

restore :
	unzip -o $(SAVEDIR)

release :
	mkdir -p $(RELEASEDIR)$(NEWRELEASEDIR)
	cp -r $(RESSOURCESDIR) $(RELEASEDIR)$(NEWRELEASEDIR)
	cp -r $(DATADIR) $(RELEASEDIR)$(NEWRELEASEDIR)
	cp -r $(BINDIR)$(EXEC) $(RELEASEDIR)$(NEWRELEASEDIR)



# $@ nom cible
# $< nom premiere dependance
# $^ nom toutes les dependances
