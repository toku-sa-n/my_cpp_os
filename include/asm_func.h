extern "C" {
void io_hlt();
void io_cli();
void io_out8(int port, int data);
int io_load_eflags();
void io_store_eflags(int eflags);

void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
}
