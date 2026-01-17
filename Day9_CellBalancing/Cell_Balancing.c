#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define NUM_CELLS 8
#define BALANCE_THRESHOLD_MV 10 // Don't balance if diff < 10mV

// Inputs: Array of cell voltages in millivolts (e.g., 3500 = 3.5V)
// Outputs: Array of boolean flags (true = turn on bleed resistor)

void Calculate_Balancing(const uint16_t *voltages, bool *balance_flags) {
    // Step 1: Find the Minimum Voltage in the pack
    // Initialize min with a safe large value (or the first element)
    uint16_t min_voltage = 0xFFFF;

    // TODO: Write Loop 1 (Find Min)
    for (int i = 0; i < NUM_CELLS; i++) {
        if (voltages[i] < min_voltage) {
            min_voltage = voltages[i];
        }
    }

    // Step 2: Decide who needs balancing
    // TODO: Write Loop 2
    // Condition: If (voltages[i] > min_voltage + BALANCE_THRESHOLD_MV) -> Turn ON
    // Else -> Turn OFF
    for (int i = 0; i < NUM_CELLS; i++) {
        if ((voltages[i] - min_voltage) > BALANCE_THRESHOLD_MV) {
            balance_flags[i] = true; // Turn ON
        } else {
            balance_flags[i] = false; // Turn OFF
        }
    }
}

int main() {
    // Scenario: Most cells are 4000mV.
    // Cell 2 is weak (3900mV).
    // Cell 5 is high (4020mV).
    uint16_t cells[NUM_CELLS] = { 4000, 4000, 3900, 4000, 4000, 4020, 4000, 3995 };
    bool flags[NUM_CELLS] = {0};

    Calculate_Balancing(cells, flags);

    printf("Cell | Voltage | Balance? | Reason\n");
    printf("-----|---------|----------|-------\n");

    for (int i = 0; i < NUM_CELLS; i++) {
        const char* reason = " - ";
        if (cells[i] == 3900) reason = "MIN CELL (Target)";
        if (flags[i]) reason = "Too High (> Min + 10mV)";
        
        printf("  %d  |  %4d   |    %d     | %s\n", 
               i, cells[i], flags[i], reason);
    }
    
    // Expectation:
    // Min is 3900. Threshold is 10. Target = 3910.
    // Any cell > 3910 should be TRUE.
    // Cell 2 (3900) -> FALSE
    // Cell 5 (4020) -> TRUE
    // Cell 7 (3995) -> TRUE
    // Others (4000) -> TRUE

    return 0;
}