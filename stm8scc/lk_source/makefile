CC  = gcc
CXX = g++
RM  = rm
MV  = mv
CP  = cp
EXE = st8lk.exe
OBJ = main.o ctrl.o lex.yy.o lnk.o common.o memory.o path.o symbol.o segment.o \
	exp.o st8link.o st8link1.o st8link2.o st8link3.o st8link4.o \
	fwriter_hex.o fwriter_map.o st8dasm.o

OPTIONS= -c -Os

$(EXE): $(OBJ) lnk.h makefile
	$(CXX) -static $(OBJ) -o $(EXE)
	$(CP) $(EXE) ../bin
	
%.o: %.c lnk.h makefile
	$(CC) $(OPTIONS) $<

%.o: %.cpp lnk.h makefile
	$(CXX) $(OPTIONS) $<

lex.yy.c: lnk.l lnk.h makefile
	flex lnk.l

lnk.h: lnk.y makefile
	bison -d lnk.y -o lnk.c

lnk.c: lnk.y makefile
	bison -d lnk.y -o lnk.c
	
clean:
	$(RM) *.o
	$(RM) lnk.h
	$(RM) lnk.c
	$(RM) lex.yy.c
	$(RM) $(EXE)
