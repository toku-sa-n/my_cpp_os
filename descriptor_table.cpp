#include "descriptor_table.h"
#include "asm_func.h"
#include "os.h"

void set_segment_descriptor(struct SegmentDescriptor* sd, unsigned int segment_size, int base, int access_right)
{
    if (segment_size > 0xfffff) {
        // When G_bit = 1, the unit of segment_size is page, not byte.
        // 1 page = 4 KB
        // G:granularity
        access_right |= 0x8000; // G_bit = 1
        segment_size /= 0x1000;
    }

    // clang-format off
    sd->limit_low    = segment_size & 0xffff;
    sd->base_low     = base & 0xffff;
    sd->base_mid     = (base >> 16) & 0xff;
    sd->access_right = access_right & 0xff;
    sd->limit_high   = ((segment_size >> 16) & 0x0f) | ((access_right >> 8) & 0xf0);
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
    struct SegmentDescriptor* gdt = (struct SegmentDescriptor*)kAddrGdt;

    // Initialize all GDT segments
    // segment_size: 0, Base: 0, Access right: 0
    for (int i = 0; i <= kLimitGdt / 8; i++) {
        set_segment_descriptor(gdt + i, 0, 0, 0);
    }

    set_segment_descriptor(gdt + 1, 0xffffffff, 0x00000000, kAddrDataRw);

    // 0x002800: The start position of OS body file
    set_segment_descriptor(gdt + 2, kLimitOSMain, kAddrOSMain, kAddrCodeEr);
    load_gdtr(kLimitGdt, kAddrGdt);

    struct GateDescriptor* idt = (struct GateDescriptor*)kAddrIdt;
    for (int i = 0; i <= kLimitIdt / 8; i++) {
        set_gate_descriptor(idt + i, 0, 0, 0);
    }
    load_idtr(kLimitIdt, kAddrIdt);

#define SET_IDT(idx) \
    set_gate_descriptor(idt + 0x##idx, (int)asm_interrupt_handler_##idx, 2 * 8, kAddrIntGate)

    SET_IDT(21);
    SET_IDT(27);
    SET_IDT(2c);
}
