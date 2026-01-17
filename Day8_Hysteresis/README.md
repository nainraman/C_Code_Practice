# Day 8 Challenge: Hysteresis (The Schmitt Trigger)

## Topic: Preventing "Chatter" in Control Loops

### The Scenario
You are controlling a cooling fan for the battery pack.

### Naive Logic
```c
if (temp > 40) fan_on(); else fan_off();
```

### The Problem
If the temperature hovers at exactly 40.0°C (reading 39.9, 40.1, 39.9...), the fan will rapidly toggle ON/OFF. This destroys the relay or the fan motor.

### The Solution (Hysteresis)
We create a "deadband" where the state does not change.

- **Turn ON**: Only when Temp > High Threshold (e.g., 40°C).
- **Turn OFF**: Only when Temp < Low Threshold (e.g., 35°C).
- **In Between (35–40°C)**: Do nothing (Stay in the current state).

### Your Goal
- Implement `Fan_Update`: Write the logic to control the `is_on` flag.
- **State Memory**: You must rely on `state->is_on` to decide what to do when you are in the "deadband."

### Code Explanation
The program simulates a fan control system with hysteresis logic. The `Fan_Update` function ensures that the fan does not toggle rapidly when the temperature hovers around the threshold values.

#### Key Functions
1. **`Fan_Init`**: Initializes the fan state.
2. **`Fan_Update`**: Updates the fan state based on the current temperature.

#### Test Scenario
The `main` function simulates a temperature profile where the temperature rises and falls. The fan state is printed at each step to verify the hysteresis logic.

### Instructions to Compile and Run

1. **Navigate to the Directory**
   ```bash
   cd /Users/ramannain/Desktop/C_Code_Practice/Day8_Hysteresis
   ```

2. **Compile the Program**
   ```bash
   gcc -o Hysteresis Hysteresis.c
   ```

3. **Run the Executable**
   ```bash
   ./Hysteresis
   ```

4. **Expected Output**
   The program will print the temperature, fan state, and expectations for each step in the temperature profile.