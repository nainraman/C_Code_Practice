# Finite State Machine Implementation

## Topic: Managing Complex Logic Flows

### The Scenario
You are writing the core logic for the Charge Controller. Charging a Lithium-Ion battery follows a strict curve:

1. **Precharge**: If the battery is very low (< 3.0V), charge slowly (0.5A) to prevent damage.
2. **Constant Current (CC)**: Blast high current (10A) until voltage hits the target (4.2V).
3. **Constant Voltage (CV)**: Hold voltage at 4.2V and let current taper off naturally.
4. **Complete**: When current drops below 0.5A, stop.
5. **Fault**: If temperature ever exceeds 45°C, stop everything immediately.

### Implementation Details
- **Transitions**: The state machine transitions between states based on voltage, current, and temperature.
- **Safety Override**: The temperature check overrides any other state.
- **Output Logic**: Based on the new state, the `cmd_current` and `cmd_voltage` are set appropriately.

### How to Compile and Run
1. Open a terminal and navigate to the directory containing the `Finite_State_Machine.c` file.
2. Compile the program using the following command:
   ```bash
   gcc -o Finite_State_Machine Finite_State_Machine.c
   ```
3. Run the executable:
   ```bash
   ./Finite_State_Machine
   ```

### Expected Output
The program simulates a sequence of events to demonstrate the state transitions:
1. **Connect Charger**: Enters Precharge state.
2. **Voltage Rises**: Transitions to CC and then CV.
3. **Current Drops**: Transitions to Complete.
4. **Overheat Event**: Transitions to Fault.

### Example Output
```
--- Test Sequence ---
State: 1 (Expect 1-Precharge) | Cmd: 0.5 A
State: 2 (Expect 2-CC)        | Cmd: 10.0 A
State: 3 (Expect 3-CV)        | Cmd: 10.0 A
State: 4 (Expect 4-Done)      | Cmd: 0.0 A
State: 5 (Expect 5-Fault)     | Cmd: 0.0 A
```