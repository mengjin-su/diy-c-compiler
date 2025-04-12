CC  = gcc
CXX = g++
RM  = rm
CP  = cp

EXE = cpp1.exe
OBJ = main.o lex.yy.o
	
OPTIONS= -c -Os 

$(EXE): $(OBJ) makefile
	$(CXX) -static $(OBJ) -o $(EXE)
	$(CP) $(EXE) ../bin	
	
lex.yy.c: cpp1.l makefile
	flex cpp1.l
	
%.o: %.c makefile
	$(CC) $(OPTIONS) $<	
	
%.o: %.cpp makefile
	$(CXX) $(OPTIONS) $<

clean:
	$(RM) *.o
	$(RM) lex.yy.c
	$(RM) $(EXE)
