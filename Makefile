BUILD_DIR		:= build

IPL_SRC			:= ipl.asm
HEAD_SRC		:= head.asm
ASM_SRC			:= asm_func.asm
CXX_SRC			:= main.cpp fonts.cpp graphics.cpp descriptor_table.cpp interrupt.cpp mouse.cpp memory.cpp
LD_SRC			:= os.ld
UTILS_SRC		:= $(shell ls utils/*.cpp|sed 's/utils\///')

IPL_FILE		:= $(addprefix $(BUILD_DIR)/, $(IPL_SRC:%.asm=%.asm.o))
HEAD_FILE		:= $(addprefix $(BUILD_DIR)/, $(HEAD_SRC:%.asm=%.asm.o))
BODY_COMPONENTS	:= $(addprefix $(BUILD_DIR)/, $(CXX_SRC:%.cpp=%.cpp.o))
BODY_COMPONENTS += $(addprefix $(BUILD_DIR)/, $(ASM_SRC:%.asm=%.asm.o))
UTILS_FILE		:= $(addprefix $(BUILD_DIR)/utils/, $(UTILS_SRC:%.cpp=%.cpp.o))

MAIN_FILE		:= $(BUILD_DIR)/os_main.o
BODY_FILE		:= $(BUILD_DIR)/os_body.o
IMG_FILE		:= $(BUILD_DIR)/os.img

DEPS			:= $(addprefix $(BUILD_DIR)/, $(CXX_SRC:%.cpp=%.cpp.d))

CXX				= g++
ASMC			= nasm
VIEWER			= qemu-system-i386
CAT				= cat

CXXFLAGS		= -std=c++17 -march=i486 -m32 -nostdlib -O2 -g -fno-pie -Wall -I ./include -ffreestanding

.SUFFIXES:

.PHONY=run

$(IMG_FILE):$(IPL_FILE) $(MAIN_FILE)|$(BUILD_DIR)
	mformat -f 1440 -C -B $(IPL_FILE) -i $@ ::
	mcopy $(MAIN_FILE) -i $@ ::

$(MAIN_FILE):$(HEAD_FILE) $(BODY_FILE)|$(BUILD_DIR)
	$(CAT) $^ > $@

$(BODY_FILE):$(BODY_COMPONENTS) $(LD_SRC)|$(BUILD_DIR)
	make -C utils/
	$(CXX) $(CXXFLAGS) -T $(LD_SRC) -o $@ $(BODY_COMPONENTS) $(UTILS_FILE)

$(BUILD_DIR)/asm_func.asm.o:asm_func.asm|$(BUILD_DIR)
	$(ASMC) -g -f elf $< -o $@

$(BUILD_DIR)/nasm_func.asm.o:nasm_func.asm|$(BUILD_DIR)
	$(ASMC) -o $@ -g -f elf $<

$(BUILD_DIR)/%.asm.o:%.asm Makefile|$(BUILD_DIR)
	$(ASMC) -o $@ $<

$(BUILD_DIR)/%.cpp.o:%.cpp Makefile|$(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -MMD -MP

$(BUILD_DIR):
	mkdir $@

run:$(IMG_FILE)
	$(VIEWER) -drive file=$<,format=raw,if=floppy

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
