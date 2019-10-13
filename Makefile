BUILD_DIR		:= build

IPL_SRC			:= ipl.asm
HEAD_SRC		:= head.asm
ASM_SRC			:= asm_func.asm
CXX_SRC			:= main.cpp
LD_SRC			:= os.ld

IPL_FILE		:= $(addprefix $(BUILD_DIR)/, $(IPL_SRC:%.asm=%.asm.o))
HEAD_FILE		:= $(addprefix $(BUILD_DIR)/, $(HEAD_SRC:%.asm=%.asm.o))
BODY_COMPONENTS	:= $(addprefix $(BUILD_DIR)/, $(CXX_SRC:%.cpp=%.cpp.o))
BODY_COMPONENTS += $(addprefix $(BUILD_DIR)/, $(ASM_SRC:%.asm=%.asm.o))

MAIN_FILE		:= $(BUILD_DIR)/os_main.o
BODY_FILE		:= $(BUILD_DIR)/os_bosy.o
IMG_FILE		:= $(BUILD_DIR)/os.img

CXX				= gcc
ASMC			= nasm
VIEWER			= qemu-system-i386
CAT				= cat

CXXFLAGS		= -march=i486 -m32 -nostdlib -O0 -g -fno-pie -Wall

.SUFFIXES:

.PHONY=run

$(IMG_FILE):$(IPL_FILE) $(MAIN_FILE) Makefile|$(BUILD_DIR)
	mformat -f 1440 -C -B $(IPL_FILE) -i $@ ::
	mcopy $(MAIN_FILE) -i $@ ::

$(MAIN_FILE):$(HEAD_FILE) $(BODY_FILE) Makefile|$(BUILD_DIR)
	$(CAT) $^ > $@

$(BODY_FILE):$(BODY_COMPONENTS) $(LD_SRC) Makefile|$(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -T $(LD_SRC) -o $@ $(BODY_COMPONENTS)

$(BUILD_DIR)/asm_func.asm.o:asm_func.asm Makefile|$(BUILD_DIR)
	$(ASMC) -g -f elf $< -o $@

$(BUILD_DIR)/%.asm.o:%.asm Makefile|$(BUILD_DIR)
	$(ASMC) -o $@ $<

$(BUILD_DIR)/%.cpp.o:%.cpp Makefile|$(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir $@

run:$(IMG_FILE)
	$(VIEWER) -drive file=$<,format=raw,if=floppy

clean:
	rm -rf $(BUILD_DIR)
