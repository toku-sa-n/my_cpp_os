#include "memory.h"
#include "asm_func.h"

// TODO: Add a more good name to CheckMemorySize_ function.

// This function actually checks the size of memory.
static unsigned CheckMemorySize_(unsigned start, unsigned end)
{
    const unsigned test_patch_0 = 0xaa55aa55, test_patch_1 = 0x55aa55aa;
    for (unsigned i = start; i <= end; i += 0x1000) {
        unsigned* p = (unsigned*)(i + 0xffc);
        unsigned old = *p;
        *p = test_patch_0;
        *p ^= 0xffffffff;

        // Check whether *p is reversed.
#define CHECK(patch)   \
    if (*p != patch) { \
        *p = old;      \
        return i;      \
    }
        CHECK(test_patch_1);

        *p ^= 0xffffffff;
        CHECK(test_patch_0);
#undef CHECK

        *p = old;
    }
    return end;
}

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

    unsigned memory_limit = CheckMemorySize_(start, end);

    if (flag_486) {
        // Enable caching.
        StoreCr0(LoadCr0() | ~kCr0CacheDisable);
    }

    return memory_limit;
}
