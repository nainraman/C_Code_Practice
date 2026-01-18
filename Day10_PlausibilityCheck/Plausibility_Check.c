#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h> // For fabsf, isnan, isinf

// Configuration
#define MAX_CURRENT_AMP  1000.0f
#define MAX_SLEW_RATE    100.0f // Max change allowed per call

// Persistent State (Static in real life, or passed in struct)
// We will use a static variable inside the function for this exercise.

// Inputs: raw_reading
// Outputs: *clean_value (only updated if valid), returns Success/Fail status
bool GetSafeCurrent(float raw_reading, float *clean_value) {
    // Static variable to remember the last accepted value
    // Initialize to 0.0 on first run
    static float last_valid = 0.0f;
    static bool first_run = true;

    // Helper for first run initialization
    if (first_run) {
        last_valid = 0.0f; // Assume we start at 0A
        first_run = false;
    }

    // 1. Check for NaN or Infinity (math.h macros)
    // TODO: If isnan(raw_reading) or isinf(raw_reading), return false.
    if(isnan(raw_reading) || isinf(raw_reading)) {
        return false;
    }

    // 2. Check Range (Hardware Limits)
    // TODO: If raw > 1000 or raw < -1000, return false.
    if(raw_reading > MAX_CURRENT_AMP || raw_reading < -MAX_CURRENT_AMP) {
        return false;
    }

    // 3. Check Slew Rate (Physics Limits)
    // Calculate delta = abs(raw - last_valid)
    // TODO: If delta > MAX_SLEW_RATE, return false.
    float delta = fabsf(raw_reading - last_valid);
    if(delta > MAX_SLEW_RATE) {
        return false;
    }

    // 4. If we survived all checks:
    // TODO:
    // Update last_valid = raw_reading
    // Update *clean_value = raw_reading
    // Return true
    last_valid = raw_reading;
    *clean_value = raw_reading;
    return true;
    
    return false; // Should not reach here if successful
}

int main() {
    float filtered_current = 0.0f;
    
    // Test Scenario
    // 1. Normal: 10.0 A
    // 2. Normal: 50.0 A (Jump of 40 is OK)
    // 3. Glitch: 2000.0 A (Out of range) -> REJECT
    // 4. Glitch: NaN (Sensor Error) -> REJECT
    // 5. Spike: 200.0 A (Jump of 150 from last valid 50) -> REJECT (Slew Rate)
    // 6. Normal: 60.0 A (Jump of 10 from last valid 50) -> ACCEPT
    
    float inputs[] = { 10.0f, 50.0f, 2000.0f, NAN, 200.0f, 60.0f };
    int num_tests = 6;

    printf("Input    | Accepted? | Output (Clean)\n");
    printf("---------|-----------|---------------\n");

    for (int i = 0; i < num_tests; i++) {
        bool result = GetSafeCurrent(inputs[i], &filtered_current);
        
        printf("%7.1f  |     %d     |    %.1f\n", 
               inputs[i], result, filtered_current);
    }
    
    // Expectation:
    // 10.0  -> Yes -> 10.0
    // 50.0  -> Yes -> 50.0
    // 2000  -> No  -> 50.0 (Hold last valid)
    // NaN   -> No  -> 50.0 (Hold last valid)
    // 200   -> No  -> 50.0 (Refuse huge jump)
    // 60.0  -> Yes -> 60.0 (10A jump from 50 is fine)

    return 0;
}