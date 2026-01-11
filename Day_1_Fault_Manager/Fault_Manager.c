#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Bit definitions
#define FAULT_OVER_VOLTAGE   (1 << 0) // 0x01
#define FAULT_UNDER_VOLTAGE  (1 << 1) // 0x02
#define FAULT_OVER_TEMP      (1 << 2) // 0x04
#define FAULT_COMM_LOSS      (1 << 3) // 0x08

// Critical faults that require opening contactors
#define CRITICAL_MASK (FAULT_OVER_VOLTAGE | FAULT_UNDER_VOLTAGE | FAULT_OVER_TEMP)

typedef struct {
    uint32_t active_faults; // Stores all current flags
} FaultManager;

static FaultManager fm;

// 1. Initialize
void FaultManager_Init(FaultManager *fm) {
    fm->active_faults = 0;
}

// 2. Helper: Set a specific fault bit
void SetFault(FaultManager *fm, uint32_t faultMask) {
    // TODO: Write the bitwise operation to set the bit
    if (faultMask == FAULT_OVER_VOLTAGE) {
        fm->active_faults |= FAULT_OVER_VOLTAGE;
    } else if (faultMask == FAULT_UNDER_VOLTAGE) {
        fm->active_faults |= FAULT_UNDER_VOLTAGE;
    } else if (faultMask == FAULT_OVER_TEMP) {
        fm->active_faults |= FAULT_OVER_TEMP;/
    } else if (faultMask == FAULT_COMM_LOSS) {
        fm->active_faults |= FAULT_COMM_LOSS;
    }
}

// 3. Helper: Clear a specific fault bit (e.g., if condition recovers)
void ClearFault(FaultManager *fm, uint32_t faultMask) {
    // TODO: Write the bitwise operation to clear the bit
    if (faultMask == FAULT_OVER_VOLTAGE) {
        fm->active_faults &= ~FAULT_OVER_VOLTAGE;
    } else if (faultMask == FAULT_UNDER_VOLTAGE) {
        fm->active_faults &= ~FAULT_UNDER_VOLTAGE;
    } else if (faultMask == FAULT_OVER_TEMP) {
        fm->active_faults &= ~FAULT_OVER_TEMP;
    } else if (faultMask == FAULT_COMM_LOSS) {
        fm->active_faults &= ~FAULT_COMM_LOSS;
    }
}

// 4. Helper: Check if a specific fault is active
bool IsFaultActive(FaultManager *fm, uint32_t faultMask) {
    // TODO: Return true if the bit is set
    if (faultMask == FAULT_OVER_VOLTAGE) {
        return (fm->active_faults & FAULT_OVER_VOLTAGE) != 0;
    } else if (faultMask == FAULT_UNDER_VOLTAGE) {
        return (fm->active_faults & FAULT_UNDER_VOLTAGE) != 0;
    } else if (faultMask == FAULT_OVER_TEMP) {
        return (fm->active_faults & FAULT_OVER_TEMP) != 0;
    } else if (faultMask == FAULT_COMM_LOSS) {
        return (fm->active_faults & FAULT_COMM_LOSS) != 0;
    }
    return false; 
}

// 5. Logic: Should we open the contactors?
// Returns true if ANY critical fault is active
bool ShouldOpenContactors(FaultManager *fm) {
    // TODO: Check if any bit in CRITICAL_MASK is set
    if ((fm->active_faults & CRITICAL_MASK) != 0) {
        return true;
    }
    return false;
}

// Simulation Test
int main() {
    FaultManager_Init(&fm);

    printf("Test 1: Setting OverVoltage...\n");
    SetFault(&fm, FAULT_OVER_VOLTAGE);
    
    if (IsFaultActive(&fm, FAULT_OVER_VOLTAGE)) printf(" -> OV Set Correctly.\n");
    if (ShouldOpenContactors(&fm)) printf(" -> Contactors Opening (Correct).\n");

    printf("Test 2: Setting Comm Loss (Non-critical)...\n");
    SetFault(&fm, FAULT_COMM_LOSS);
    
    // Clear the critical fault
    ClearFault(&fm, FAULT_OVER_VOLTAGE);
    
    if (ShouldOpenContactors(&fm)) {
        printf(" -> FAIL: Contactors still open but only Comm Loss is present.\n");
    } else {
        printf(" -> SUCCESS: Contactors closed (Comm Loss is not critical).\n");
    }

    return 0;
}