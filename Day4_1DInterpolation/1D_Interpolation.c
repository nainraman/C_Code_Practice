#include <stdio.h>
#include <stdint.h>

// Simulated OCV Table (simplified LFP curve)
// Voltage must be strictly increasing for this logic to work.
#define TABLE_SIZE 5

// Known Voltages (X-axis)
const float LUT_Voltage[TABLE_SIZE] = { 3.00f, 3.20f, 3.25f, 3.30f, 3.60f };

// Corresponding SOC (Y-axis)
const float LUT_SOC[TABLE_SIZE]     = { 0.0f,  30.0f, 60.0f, 80.0f, 100.0f };

float GetSOC_From_Voltage(float voltage) {
    // 1. Check Lower Bound (Safety)
    // TODO: If voltage <= LUT_Voltage[0], return the first SOC value.
    if (voltage <= LUT_Voltage[0]) {
        return LUT_SOC[0];
    }

    // 2. Check Upper Bound (Safety)
    // TODO: If voltage >= LUT_Voltage[last], return the last SOC value.
    if (voltage >= LUT_Voltage[TABLE_SIZE - 1]) {
        return LUT_SOC[TABLE_SIZE - 1];
    }

    // 3. Find the Interval and Interpolate
    for (int i = 0; i < TABLE_SIZE - 1; i++) {
        float v_low  = LUT_Voltage[i];
        float v_high = LUT_Voltage[i+1];

        // Check if our voltage is between these two points
        if (voltage >= v_low && voltage <= v_high) {
            float soc_low  = LUT_SOC[i];
            float soc_high = LUT_SOC[i+1];

            // TODO: Implement Linear Interpolation Formula
            float soc = soc_low + ( (voltage - v_low) / (v_high - v_low) ) * (soc_high - soc_low);
            return soc;
            // Return calculated SOC
            return 0.0f; // placeholder
        }
    }

    return 0.0f; // Should not reach here if bounds are handled
}

int main() {
    // Test Cases
    float test_volts[] = { 2.9f, 3.0f, 3.1f, 3.225f, 3.6f, 3.7f };
    // Expected:
    // 2.9V   -> 0.0% (Clamped low)
    // 3.0V   -> 0.0% (Exact match)
    // 3.1V   -> 15.0% (Midpoint between 3.0 and 3.2)
    // 3.225V -> 45.0% (Midpoint between 3.2[30%] and 3.25[60%])
    // 3.6V   -> 100.0% (Exact match)
    // 3.7V   -> 100.0% (Clamped high)

    for (int i = 0; i < 6; i++) {
        float v = test_volts[i];
        float soc = GetSOC_From_Voltage(v);
        printf("Voltage: %.3f V -> SOC: %.2f%%\n", v, soc);
    }

    return 0;
}