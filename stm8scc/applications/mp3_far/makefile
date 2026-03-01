CC=st8cc
AS=st8as
LK=st8lk
RM=rm

HEX=mp3.hex
OBJ=main.obj spi.obj define.obj fat.obj key.obj vs1011.obj play.obj mmcspi.obj timer.obj sound.obj

$(HEX):$(OBJ) makefile
	$(LK) -E $(OBJ) -o mp3

%.obj: %.asm
	$(AS) $<
	
%.asm: %.c
	$(CC) -E $<

clean:
	$(RM) *.obj
	$(RM) $(HEX)
	$(RM) *.lst
