#include "include/multi_core.h"

// core_boot geeft pointer naar write bootadresregister van core 'core_id'
volatile core_reg_t *core_boot(int core_id) {
    return (volatile core_reg_t *)((uintptr_t)(BASE_ADDR + (CORE_STRIDE * core_id)));
}
