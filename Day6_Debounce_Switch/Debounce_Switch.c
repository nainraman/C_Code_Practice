#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t counter;       // "Integrator" value (0 to 100)
    bool stable_state;     // The official, filtered output state
} Debouncer;

// Configuration
#define COUNTER_MAX 100
#define THRESHOLD_HIGH 80  // Must reach this to turn ON
#define THRESHOLD_LOW  20  // Must drop to this to turn OFF

void Debounce_Init(Debouncer *db) {
    db->counter = 0;
    db->stable_state = false;
}

// Run this every 10ms
void Debounce_Update(Debouncer *db, bool raw_input) {
    // 1. Update Counter based on raw input
    if (raw_input == true) {
        // Increment counter, but clamp at COUNTER_MAX
        // TODO: db->counter++ (careful of overflow)
        if (db->counter < COUNTER_MAX) {
            db->counter++;
        }
    } else {
        // Decrement counter, but clamp at 0
        // TODO: db->counter-- (careful of underflow)
        if (db->counter > 0) {
            db->counter--;
        }
    }

    // 2. Determine Output State (Hysteresis)
    // TODO:
    // If counter >= THRESHOLD_HIGH -> stable_state = true
    // If counter <= THRESHOLD_LOW  -> stable_state = false
    // Else -> keep current state
    if (db->counter >= THRESHOLD_HIGH) {
        db->stable_state = true;
    } else if (db->counter <= THRESHOLD_LOW) {
        db->stable_state = false;
    }
}

int main() {
    Debouncer db;
    Debounce_Init(&db);

    printf("--- Test 1: Rising Edge with Noise ---\n");
    // Simulate a bounce: 1, 0, 1, 1, 1...
    // We expect the state to stay 0 until enough '1's accumulate.
    
    bool inputs[] = {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // 11 steps
    
    for (int i = 0; i < 11; i++) {
        Debounce_Update(&db, inputs[i]);
        printf("Input: %d | Counter: %3d | State: %d\n", 
               inputs[i], db.counter, db.stable_state);
    }
    
    // NOTE: In this short test, we might not reach 80 (800ms) to flip the state.
    // That is INTENTIONAL. Real debouncing takes time. 
    // To verify it works, simulate a long stream of 1s below.

    printf("\n--- Test 2: Solid High Signal (Fast Forward) ---\n");
    for (int i = 0; i < 80; i++) {
        Debounce_Update(&db, 1);
    }
    printf("After 800ms HIGH -> Counter: %d | State: %d (Expect 1)\n", 
           db.counter, db.stable_state);

    printf("\n--- Test 3: Falling Edge with Noise ---\n");
    // Now signal drops to 0, but glitches to 1 briefly
    bool inputs_drop[] = {0, 0, 1, 0, 0, 0, 0}; 
    for (int i = 0; i < 7; i++) {
        Debounce_Update(&db, inputs_drop[i]);
    }
    printf("After noisy drop -> Counter: %d | State: %d (Expect 1 still)\n", 
           db.counter, db.stable_state);
           
    return 0;
}