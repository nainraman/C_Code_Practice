#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Configuration
#define TEMP_TURN_ON   40.0f
#define TEMP_TURN_OFF  35.0f

typedef struct {
    bool is_on; // Memory: Is the fan currently running?
} FanState;

void Fan_Init(FanState *fan) {
    fan->is_on = false;
}

// Run this periodically (e.g., every 1 second)
void Fan_Update(FanState *fan, float current_temp) {
    // TODO: Implement Hysteresis Logic
    // 1. If temp > TURN_ON, force ON.
    // 2. If temp < TURN_OFF, force OFF.
    // 3. If in between... do NOT touch the state.
    if (current_temp > TEMP_TURN_ON) {
        fan->is_on = true;
    } else if (current_temp < TEMP_TURN_OFF) {
        fan->is_on = false;
    }
}

int main() {
    FanState fan;
    Fan_Init(&fan);

    // Test Scenario: Temperature rising then falling
    float temp_profile[] = { 
        30.0, 34.0, 38.0, 41.0,  // Rising (should turn ON at 41)
        39.0, 36.0, 34.0, 30.0   // Falling (should stay ON until 34)
    };
    
    int steps = sizeof(temp_profile) / sizeof(float);

    printf("Temp | Fan State | Expectation\n");
    printf("-----|-----------|------------\n");

    for (int i = 0; i < steps; i++) {
        float t = temp_profile[i];
        Fan_Update(&fan, t);
        
        const char* note = "";
        if (t == 41.0) note = "<- Trigger ON";
        if (t == 39.0) note = "<- Holding ON (Deadband)";
        if (t == 34.0) note = "<- Trigger OFF";

        printf("%.1f |     %d     | %s\n", t, fan.is_on, note);
    }

    return 0;
}