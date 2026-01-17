# Debounce Switch Implementation

## Topic: Signal Conditioning / Time-Domain Filtering

### The Scenario
You are reading a "High Voltage Interlock Loop" (HVIL) signal. It's a physical switch that detects if the battery cover is open. When a mechanic opens the cover, the switch might mechanically "bounce" (rapidly toggle 1-0-1-0-1) for 10-20 milliseconds before settling at 0.

### The Risk
If you act on the first "0", you might trip the system. If you immediately see a "1" again, you might try to re-engage high voltage while the contactor is moving. Arc flash risk!

### The Solution (Integrator Method)
We only trust the signal if it has been stable for a specific amount of time.

#### Counter Strategy
- If `Raw_Input == 1`, increment counter (up to a MAX).
- If `Raw_Input == 0`, decrement counter (down to 0).

#### Threshold
- If `counter > Threshold`, consider state HIGH.
- If `counter < Threshold`, consider state LOW.

### Implementation Details
- **Debounce_Update**: This function runs every 10ms.
- **Hysteresis**: To prevent "flickering" right at the threshold, use two thresholds (like a Schmitt Trigger).
  - Switch to HIGH state only if `counter >= 80`.
  - Switch to LOW state only if `counter <= 20`.
  - Range (20, 80): Keep previous state (Stable).

### How to Compile and Run
1. Open a terminal and navigate to the directory containing the `Debounce_Switch.c` file.
2. Compile the program using the following command:
   ```bash
   gcc -o Debounce_Switch Debounce_Switch.c
   ```
3. Run the executable:
   ```bash
   ./Debounce_Switch
   ```

### Expected Output
The program simulates three scenarios:
1. **Rising Edge with Noise**: Demonstrates how the counter accumulates and the state remains stable until the threshold is crossed.
2. **Solid High Signal**: Fast-forwards through a stable high signal to show the state transition.
3. **Falling Edge with Noise**: Shows how the state remains stable despite brief noise.

### Example Output
```
--- Test 1: Rising Edge with Noise ---
Input: 1 | Counter:   1 | State: 0
Input: 0 | Counter:   0 | State: 0
Input: 1 | Counter:   1 | State: 0
...

--- Test 2: Solid High Signal (Fast Forward) ---
After 800ms HIGH -> Counter: 80 | State: 1 (Expect 1)

--- Test 3: Falling Edge with Noise ---
After noisy drop -> Counter: 19 | State: 1 (Expect 1 still)
```