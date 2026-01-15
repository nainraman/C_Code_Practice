#include <stdio.h>
#include <stdint.h>

// Configuration
// Alpha = 0.1. In Fixed Point (Scale 1024): 0.1 * 1024 = 102.4 -> 102
#define SCALE_SHIFT 10      // 2^10 = 1024
#define SCALE_FACTOR (1 << SCALE_SHIFT)
#define ALPHA_FIXED  102    // Approx 0.1
#define ONE_MINUS_ALPHA (SCALE_FACTOR - ALPHA_FIXED) // 922

// 1. Reference: Slow Floating Point Implementation
float EMA_Update_Float(float input, float current_avg) {
    float alpha = 0.1f;
    return (alpha * input) + ((1.0f - alpha) * current_avg);
}

// 2. Task: Fast Fixed Point Implementation
int32_t EMA_Update_Fixed(int32_t input, int32_t current_avg) {
    // TODO: Implement the EMA formula using only integers.
    // Formula: (ALPHA * input + (SCALE - ALPHA) * current_avg) / SCALE
    
    // Steps:
    // 1. Calculate the numerator (using multiplication)
    // 2. Perform division using bit-shifting (>> SCALE_SHIFT)
    // 3. Optional Challenge: Add rounding (add SCALE_FACTOR / 2 before shifting)
    int32_t numerator = (ALPHA_FIXED * input) + (ONE_MINUS_ALPHA * current_avg);
    int32_t result = (numerator + (SCALE_FACTOR / 2)) >> SCALE_SHIFT; // Rounding
    return result;
    
    return 0; // Placeholder
}

int main() {
    // Scenario: Battery Voltage in millivolts (mV)
    // Start with a baseline of 3300 mV
    float f_avg = 3300.0f;
    int32_t i_avg = 3300; 

    // Test Inputs: A jump to 3500mV, then staying there
    int32_t inputs[] = { 3500, 3500, 3500, 3500, 3500 };

    printf("Iter | Float Avg | Fixed Avg | Error\n");
    printf("-----|-----------|-----------|------\n");

    for (int i = 0; i < 5; i++) {
        // Run Float Version
        f_avg = EMA_Update_Float((float)inputs[i], f_avg);

        // Run Fixed Version
        i_avg = EMA_Update_Fixed(inputs[i], i_avg);

        // Compare
        printf("  %d  |  %.2f  |   %d   | %.2f\n", 
               i+1, f_avg, i_avg, f_avg - (float)i_avg);
    }
    
    // Note: Small errors (off by 1) are acceptable due to integer truncation.
    // Large errors mean the math is wrong.

    return 0;
}