#pragma once

extern "C" {
void IoHlt();
void IoCli();
void IoSti();
void IoStiHlt();
void IoOut8(int port, int data);
int IoIn8(int port);
int IoLoadEflags();
void IoStoreEflags(int eflags);

void LoadGdtr(int limit, int addr);
void LoadIdtr(int limit, int addr);

void AsmInterruptHandler21();
void AsmInterruptHandler27();
void AsmInterruptHandler2c();

int LoadCr0();
void StoreCr0(int cr0);

unsigned AsmCheckMemorySize(unsigned start, unsigned end);
}
