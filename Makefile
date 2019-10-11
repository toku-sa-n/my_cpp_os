IPL_SRC			:= ipl.asm
MAIN_SRC		:= main.asm

IPL_FILE		:= $(IPL_SRC:%.asm=%.asm.o)
MAIN_FILE		:= $(MAIN_SRC:%.asm=%.asm.o)

IMG_FILE		:= os.img

ASMC			= nasm
VIEWER			= qemu-system-i386
CAT				= cat

.SUFFIXES:

.PHONY=run

$(IMG_FILE):$(IPL_FILE) $(MAIN_FILE)
	$(CAT) $^ > $@

%.asm.o:%.asm
	$(ASMC) -o $@ $<

run:os.img
	$(VIEWER) -drive file=$<,format=raw,if=floppy
