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
	$(CAT) $^ > $@

$(BUILD_DIR)/%.asm.o:%.asm|$(BUILD_DIR)
	$(ASMC) -o $@ $<

$(BUILD_DIR):
	mkdir $@

run:os.img
	$(VIEWER) -drive file=$<,format=raw,if=floppy
