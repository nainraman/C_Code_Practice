#include <stdio.h>
#include <stdint.h>

// State Definitions
typedef enum {
    STATE_IDLE,
    STATE_PRECHARGE,
    STATE_CC,        // Constant Current
    STATE_CV,        // Constant Voltage
    STATE_COMPLETE,
    STATE_FAULT
} ChargeState_t;

// System Inputs (Sensors)
typedef struct {
    float voltage;
    float current;
    float temp_c;
    int charger_connected;
} Sensors;

// System Outputs (Commands to Charger)
typedef struct {
    float cmd_current_limit;
    float cmd_voltage_limit;
} ChargerCmd;

// Persistent Context
typedef struct {
    ChargeState_t state;
} ChargeController;

// Configuration
#define V_PRECHARGE_EXIT  3.0f
#define V_TARGET          4.2f
#define I_TERMINATION     0.5f
#define TEMP_MAX          45.0f

void Controller_Init(ChargeController *cc) {
    cc->state = STATE_IDLE;
}

void Controller_Run(ChargeController *cc, Sensors *sens, ChargerCmd *cmd) {
    // 1. Global Safety Check (Highest Priority)
    // TODO: If sens->temp_c > TEMP_MAX, force state to STATE_FAULT immediately.
    if (sens->temp_c > TEMP_MAX) {
        cc->state = STATE_FAULT;
    }

    // 2. State Machine Logic
    switch (cc->state) {
        case STATE_IDLE:
            cmd->cmd_current_limit = 0.0f;
            cmd->cmd_voltage_limit = 0.0f;
            
            // Transition: If charger is connected...
            if (sens->charger_connected) {
                // Check voltage to decide if we need Precharge or CC
                // TODO: If V < V_PRECHARGE_EXIT go to PRECHARGE, else go to CC
                if (sens->voltage < V_PRECHARGE_EXIT) {
                    cc->state = STATE_PRECHARGE;
                } else {
                    cc->state = STATE_CC;
                }
            }
            break;

        case STATE_PRECHARGE:
            cmd->cmd_current_limit = 0.5f; // Low current
            cmd->cmd_voltage_limit = 4.2f; // Max voltage
            
            // TODO: Transition to CC if voltage rises above V_PRECHARGE_EXIT
            if (sens->voltage >= V_PRECHARGE_EXIT) {
                cc->state = STATE_CC;
            }
            break;

        case STATE_CC:
            cmd->cmd_current_limit = 10.0f; // Fast charge
            cmd->cmd_voltage_limit = 4.2f;
            
            // TODO: Transition to CV if voltage >= V_TARGET
            if (sens->voltage >= V_TARGET) {
                cc->state = STATE_CV;
            }
            break;

        case STATE_CV:
            cmd->cmd_current_limit = 10.0f; // Allow up to max, but battery will draw less
            cmd->cmd_voltage_limit = 4.2f;  // Hold strictly at 4.2V
            
            // TODO: Transition to COMPLETE if current < I_TERMINATION
            if (sens->current < I_TERMINATION) {
                cc->state = STATE_COMPLETE;
            }
            break;

        case STATE_COMPLETE:
            cmd->cmd_current_limit = 0.0f;
            cmd->cmd_voltage_limit = 0.0f;
            // Stay here until charger disconnected (optional reset logic)
            break;

        case STATE_FAULT:
            cmd->cmd_current_limit = 0.0f;
            cmd->cmd_voltage_limit = 0.0f;
            printf("!! FAULT ACTIVE !!\n");
            // Stay here forever (latched fault)
            break;
    }
}

int main() {
    ChargeController cc;
    Sensors sens = {2.5f, 0.0f, 25.0f, 0}; // Start: 2.5V, Room Temp, Disconnected
    ChargerCmd cmd = {0};
    
    Controller_Init(&cc);

    printf("--- Test Sequence ---\n");
    
    // 1. Connect Charger (Should enter Precharge)
    sens.charger_connected = 1;
    Controller_Run(&cc, &sens, &cmd);
    printf("State: %d (Expect 1-Precharge) | Cmd: %.1f A\n", cc.state, cmd.cmd_current_limit);

    // 2. Voltage rises to 3.1V (Should enter CC)
    sens.voltage = 3.1f;
    Controller_Run(&cc, &sens, &cmd);
    printf("State: %d (Expect 2-CC)        | Cmd: %.1f A\n", cc.state, cmd.cmd_current_limit);

    // 3. Voltage hits 4.2V (Should enter CV)
    sens.voltage = 4.2f;
    Controller_Run(&cc, &sens, &cmd);
    printf("State: %d (Expect 3-CV)        | Cmd: %.1f A\n", cc.state, cmd.cmd_current_limit);

    // 4. Current drops to 0.4A (Should enter Complete)
    sens.current = 0.4f;
    Controller_Run(&cc, &sens, &cmd);
    printf("State: %d (Expect 4-Done)      | Cmd: %.1f A\n", cc.state, cmd.cmd_current_limit);

    // 5. Overheat Event
    sens.temp_c = 50.0f;
    Controller_Run(&cc, &sens, &cmd);
    printf("State: %d (Expect 5-Fault)     | Cmd: %.1f A\n", cc.state, cmd.cmd_current_limit);

    return 0;
}