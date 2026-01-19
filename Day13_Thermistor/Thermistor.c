#include <stdio.h>
#include <stdint.h>
#include <math.h>

// Hardware Configuration
#define V_REF           5.0f
#define ADC_MAX         4095.0f
#define R_PULLUP        10000.0f // 10k Ohm

// Thermistor Datasheet (NTC 10k @ 25C, Beta 3435)
#define R_25            10000.0f
#define T_25_KELVIN     (25.0f + 273.15f)
#define BETA            3435.0f

// Error Code
#define TEMP_ERROR      -999.0f

float GetTemp_C(uint16_t adc_raw) {
    // 1. Safety Check: Detect Open/Short circuit
    // TODO: If adc_raw is 0 or >= 4095, return TEMP_ERROR.
    if (adc_raw == 0 || adc_raw >= 4095) {
        return TEMP_ERROR;
    }
    
    // 2. Convert ADC to Voltage
    // float v_out = ...
    float v_out = (adc_raw / ADC_MAX) * V_REF;
    
    // 3. Calculate Thermistor Resistance (R_ntc)
    // Formula: R_ntc = R_pullup * (v_out / (V_REF - v_out))
    // Note: If V_REF - v_out is near zero, your safety check above should have caught it.
    float r_ntc = R_PULLUP * (v_out / (V_REF - v_out));
    
    // 4. Calculate Temperature (Kelvin) using Beta Parameter Equation
    // Formula: 1 / ( (1/T_25) + (1/Beta) * ln(R_ntc / R_25) )
    // Use log() for natural logarithm.
    float temp_kelvin = 1.0f / ( (1.0f / T_25_KELVIN) + (1.0f / BETA) * logf(r_ntc / R_25) );
    
    // 5. Convert to Celsius and return
    return temp_kelvin - 273.15f;
}

int main() {
    // Test Cases
    // 2048 counts ~ 2.5V. If Vout=2.5V and Pullup=10k, then NTC=10k.
    // NTC=10k is exactly 25C.
    uint16_t test_inputs[] = { 0, 4095, 2048, 1000, 3000 };
    
    printf("ADC Counts | Temp (C) | Expected Note\n");
    printf("-----------|----------|--------------\n");
    
    for (int i = 0; i < 5; i++) {
        float t = GetTemp_C(test_inputs[i]);
        printf("   %4d    |  %6.1f  | ", test_inputs[i], t);
        
        if (t == -999.0f) printf("Fault Detected\n");
        else if (fabsf(t - 25.0f) < 1.0f) printf("Approx 25C (Room Temp)\n");
        else if (t > 25.0f) printf("Hotter (Lower Resistance)\n");
        else printf("Colder (Higher Resistance)\n");
    }

    return 0;
}