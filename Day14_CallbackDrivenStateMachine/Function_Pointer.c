#include <stdio.h>
#include <stdint.h>

// 1. Define State Indices
typedef enum {
    STATE_IDLE = 0,
    STATE_ACTIVE,
    STATE_FAULT,
    // Helper to know how many states we have
    NUM_STATES
} State_t;

// Global System State
State_t current_state = STATE_IDLE;
uint32_t system_counter = 0;

// 2. Function Prototypes (Must match the typedef)
// typedef for a function that takes no args and returns void
typedef void (*StateHandler)(void);

// Forward declarations of our state functions
void Handler_Idle(void);
void Handler_Active(void);
void Handler_Fault(void);

// 3. The Jump Table (Array of Function Pointers)
// TODO: Create the array 'State_Table'
// const StateHandler State_Table[NUM_STATES] = { ... };
const StateHandler State_Table[NUM_STATES] = {
    // TODO: Fill this in order matching the Enum
    Handler_Idle,
    Handler_Active,
    Handler_Fault
};

// --- State Implementations ---

void Handler_Idle(void) {
    printf("[IDLE] Waiting... (Count: %d)\n", system_counter);
    system_counter++;
    
    // TODO: If counter > 2, transition to ACTIVE
    if (system_counter > 2) {
        current_state = STATE_ACTIVE;
    }
}

void Handler_Active(void) {
    printf("[ACTIVE] Doing heavy work...\n");
    // TODO: Reset counter to 0
    system_counter = 0;
    // TODO: Transition to FAULT (Simulating a crash)
    current_state = STATE_FAULT;
}

void Handler_Fault(void) {
    printf("[FAULT] Clearing errors...\n");
    // TODO: Transition back to IDLE
    current_state = STATE_IDLE;
}

// --- Main Loop ---

int main() {
    printf("--- Final Challenge: Function Pointer FSM ---\n");

    // Simulate 10 system ticks
    for (int i = 0; i < 10; i++) {
        // 4. The Magic Line
        // Instead of switch/case, we index the table directly.
        // Safety: Ensure current_state is valid!
        
        if (current_state < NUM_STATES) {
             // TODO: Call the function from the table
             // State_Table[current_state]();
            State_Table[current_state]();
        }
    }

    return 0;
}