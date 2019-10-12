BUILD_DIR		:= build

IPL_SRC			:= ipl.asm
MAIN_SRC		:= main.asm

IPL_FILE		:= $(addprefix $(BUILD_DIR)/, $(IPL_SRC:%.asm=%.asm.o))
MAIN_FILE		:= $(addprefix $(BUILD_DIR)/, $(MAIN_SRC:%.asm=%.asm.o))

IMG_FILE		:= $(BUILD_DIR)/os.img

ASMC			= nasm
VIEWER			= qemu-system-i386
CAT				= cat

.SUFFIXES:

.PHONY=run

$(IMG_FILE):$(IPL_FILE) $(MAIN_FILE)|$(BUILD_DIR)
	mformat -f 1440 -C -B $(IPL_FILE) -i $@ ::
	mcopy $(MAIN_FILE) -i $@ ::

$(BUILD_DIR)/%.asm.o:%.asm|$(BUILD_DIR)
	$(ASMC) -o $@ $<

$(BUILD_DIR):
	mkdir $@

run:$(IMG_FILE)
	$(VIEWER) -drive file=$<,format=raw,if=floppy

clean:
	rm -rf $(BUILD_DIR)
