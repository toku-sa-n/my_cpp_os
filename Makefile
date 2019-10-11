os.img:ipl.asm
	nasm $< -o $@

run:os.img
	qemu-system-i386 -drive file=$<,format=raw,if=floppy

.PHONY=run
