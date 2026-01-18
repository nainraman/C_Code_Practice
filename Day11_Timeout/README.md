# Day 11 Challenge: Timeout Implementation (Non-Blocking)

## Topic: Real-Time Constraints & State Machine Timing

### The Scenario
You send a CAN message: "Start Charging". You expect a response: "OK, Starting".

### The Problem
If the charger is unplugged, broken, or the CAN bus is severed, you will never get a response.

### The Wrong Way
Using `while (!response) {}` or `delay(1000)` hangs the CPU. The watchdog timer will kill your car.

### The Right Way
A Non-Blocking Timer inside your state machine. You count "ticks" every time the function is called.

### Your Goal

1. **Implement the Timer**: Increment a counter only while in the `WAIT_RESP` state.
2. **Transitions**:
   - **Success**: If `IsResponseReceived()` is true, go to `STATE_CHARGING`.
   - **Failure**: If `timer > MAX_TICKS`, go to `STATE_ERROR`.
   - **Reset**: Crucial—remember to reset the timer to 0 before entering the wait state.

### How to Compile and Run

1. Open a terminal and navigate to the directory containing the `Timeout_Logic.c` file.
2. Compile the code using the following command:
   ```bash
   gcc -o Timeout_Logic Timeout_Logic.c
   ```
3. Make the executable file runnable (if required):
   ```bash
   chmod +x Timeout_Logic
   ```
4. Run the executable:
   ```bash
   ./Timeout_Logic
   ```
5. Observe the output in the terminal to understand the state transitions and timeout behavior.