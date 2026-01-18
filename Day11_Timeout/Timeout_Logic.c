#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Configuration
#define TIMEOUT_MS      1000  // 1 second timeout
#define TASK_PERIOD_MS  10    // This function runs every 10ms
#define MAX_TICKS       (TIMEOUT_MS / TASK_PERIOD_MS) // 100 ticks

typedef enum {
    STATE_IDLE,
    STATE_SEND_CMD,
    STATE_WAIT_RESP,
    STATE_CHARGING,
    STATE_ERROR
} CommState;

typedef struct {
    CommState state;
    uint32_t timer_ticks;
} CommSystem;

// Mock Hardware Function
bool IsResponseReceived() {
    // In real code, this checks a CAN buffer.
    // We will simulate this in main().
    extern bool mock_response_flag; 
    return mock_response_flag;
}

bool mock_response_flag = false; // Global for simulation

void Comm_Update(CommSystem *sys) {
    switch (sys->state) {
        case STATE_IDLE:
            // Auto-start for this test
            sys->state = STATE_SEND_CMD;
            break;

        case STATE_SEND_CMD:
            printf("[Command Sent] -> Waiting for reply...\n");
            // TODO: Reset timer to 0
            // TODO: Move to STATE_WAIT_RESP
            sys->timer_ticks = 0;
            sys->state = STATE_WAIT_RESP;
            break;

        case STATE_WAIT_RESP:
            // TODO: Increment timer
            sys->timer_ticks++;
            
            // TODO: Check if response received
            // If yes -> Go to STATE_CHARGING
            if (IsResponseReceived()) {
                sys->state = STATE_CHARGING;
                break;
            }
            
            // TODO: Check for Timeout
            // If timer > MAX_TICKS -> Go to STATE_ERROR
            if (sys->timer_ticks > MAX_TICKS) {
                sys->state = STATE_ERROR;
            }
            break;

        case STATE_CHARGING:
            printf("[Charging] All systems go.\n");
            break;

        case STATE_ERROR:
            printf("[Error] Timeout! Charger did not reply.\n");
            // Stuck here
            break;
    }
}

int main() {
    CommSystem sys = {0};
    sys.state = STATE_IDLE;

    printf("--- Test 1: Timeout Scenario (Charger is dead) ---\n");
    mock_response_flag = false; // Charger will NEVER reply
    
    // Run for 110 iterations (1.1 seconds)
    // We expect it to hit ERROR around tick 100.
    for (int i = 0; i < 110; i++) {
        if (sys.state == STATE_ERROR) break; // Stop if we hit error
        
        Comm_Update(&sys);
        
        // Print status every 20 ticks to keep output clean
        if (i % 20 == 0) printf("  Tick %d: State %d\n", i, sys.state);
    }

    printf("\n--- Test 2: Success Scenario ---\n");
    // Reset
    sys.state = STATE_IDLE;
    sys.timer_ticks = 0;
    mock_response_flag = false;

    // Run for 50 ticks, then simulate a reply
    for (int i = 0; i < 100; i++) {
        if (i == 50) {
            printf("  !!! SIMULATING REPLY RECEIVED !!!\n");
            mock_response_flag = true;
        }
        
        Comm_Update(&sys);
        
        if (sys.state == STATE_CHARGING) {
            printf("  Tick %d: Entered Charging State! (Success)\n", i);
            break;
        }
    }

    return 0;
}