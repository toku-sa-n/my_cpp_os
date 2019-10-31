#include "memory.h"
#include "asm_func.h"

// This function checks the architecture.
// If the architecture has cache memories, this function disables it.
// Because if program tries to change the value of memory, it actually changes the cache's value.
// The actual size check will be done in CheckMemorySize_ function.
unsigned CheckMemorySize(unsigned start, unsigned end)
{
    // Check whether the architecture is 386, 486 or newer.
    IoStoreEflags(IoLoadEflags() | kEflagsAcBit);
    bool flag_486 = false;
    char eflag = IoLoadEflags();

    // On 386 arch, AC flag is always 0.
    if (eflag & kEflagsAcBit) {
        flag_486 = true;
    }

    IoStoreEflags(eflag & ~kEflagsAcBit);

    if (flag_486) {
        // Disable caching.
        StoreCr0(LoadCr0() | kCr0CacheDisable);
    }

    unsigned memory_limit = AsmCheckMemorySize(start, end);

    if (flag_486) {
        // Enable caching.
        StoreCr0(LoadCr0() | ~kCr0CacheDisable);
    }

    return memory_limit;
}
