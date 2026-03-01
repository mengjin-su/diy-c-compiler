CC=st8cc
AS=st8as
LK=st8lk
RM=rm

HEX=led.hex
OBJ=main.obj

$(HEX):$(OBJ) makefile
	$(LK) -E $(OBJ) -o led

%.obj: %.asm
	$(AS) $<
	
%.asm: %.c
	$(CC) -E $<

clean:
	$(RM) *.obj
	$(RM) $(HEX)
	$(RM) *.lst


