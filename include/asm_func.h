extern "C" {
void IoHlt();
void IoCli();
void IoSti();
void IoOut8(int port, int data);
int IoIn8(int port);
int IoLoadEflags();
void IoStoreEflags(int eflags);

void LoadGdtr(int limit, int addr);
void LoadIdtr(int limit, int addr);

void AsmInterruptHandler21();
void AsmInterruptHandler27();
void AsmInterruptHandler2c();
}
