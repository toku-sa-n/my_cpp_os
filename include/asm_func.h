extern "C" {
void io_hlt();
void io_cli();
void io_sti();
void io_out8(int port, int data);
int io_in8(int port);
int io_load_eflags();
void io_store_eflags(int eflags);

void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

void asm_interrupt_handler_21();
void asm_interrupt_handler_27();
void asm_interrupt_handler_2c();
}
