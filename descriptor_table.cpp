#include "asm_func.h"
#include "os.h"

void set_segment_descriptor(struct SegmentDescriptor* sd, unsigned int limit, int base, int access_right)
{
    if (limit > 0xfffff) {
        access_right |= 0x8000;
        limit /= 0x1000;
    }

    // clang-format off
    sd->limit_low    = limit & 0xffff;
    sd->base_low     = base & 0xffff;
    sd->base_mid     = (base >> 16) & 0xff;
    sd->access_right = access_right & 0xff;
    sd->limit_high   = ((limit >> 16) & 0x0f) | ((access_right >> 8) & 0xf0);
    sd->base_high    = (base >> 24) & 0xff;
    // clang-format on
}

void set_gate_descriptor(struct GateDescriptor* gd, int offset, int selector, int access_right)
{
    // clang-format off
    gd->offset_low   = offset & 0xffff;
    gd->selector     = selector;
    gd->dw_count     = (access_right >> 8) & 0xff;
    gd->access_right = access_right & 0xff;
    gd->offset_high  = (offset >> 16) & 0xffff;
    // clang-format on
}

void init_gdt_idt()
{
    // 0x270000 ~ 0x27ffff: GDT
    struct SegmentDescriptor* gdt = (struct SegmentDescriptor*)0x00270000;

    // Initialize all GDT segments
    // Limit: 0, Base: 0, Access right: 0
    for (int i = 0; i < 8192; i++) {
        set_segment_descriptor(gdt + i, 0, 0, 0);
    }

    set_segment_descriptor(gdt + 1, 0xffffffff, 0x00000000, 0x4092);

    // 0x002800: The start position of OS body file
    set_segment_descriptor(gdt + 2, 0x0007ffff, 0x00280000, 0x409a);
    load_gdtr(0xffff, 0x00270000);

    struct GateDescriptor* idt = (struct GateDescriptor*)0x0026f800;
    for (int i = 0; i < 256; i++) {
        set_gate_descriptor(idt + i, 0, 0, 0);
    }
    load_idtr(0x7ff, 0x0026f800);
}
