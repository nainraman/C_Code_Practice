# Cell Balancing Logic

## Topic: Array Iteration & BMS Logic

### The Scenario
A battery pack has 96 cells in series. They must all be at the same voltage to maximize capacity. If Cell #1 is at 4.0V and Cell #2 is at 4.2V (Full), we cannot charge the pack anymore because Cell #2 would overcharge. Solution: We turn on a "bleed resistor" for Cell #2 to drain it down to match Cell #1. This is Passive Balancing.

### The Logic

1. **Find the Target**: The "weakest link" determines the pack status. In balancing, we usually want to bring everyone down to the level of the Minimum Cell Voltage.
2. **Threshold**: We don't balance if the difference is tiny (e.g., < 10mV). It's not worth the heat generation.
3. **Safety**: Never balance a cell that is already at the minimum.

### Algorithm

- **Two-Pass Algorithm**:
  - **Pass 1**: Iterate through all cells to find the global `min_voltage`.
  - **Pass 2**: Iterate again. If `cell[i] > min_voltage + THRESHOLD`, enable balancing for that cell.

#### Optimization Question
Why can't we do this in a single loop? (Think about it while coding).

### Instructions to Compile and Run

1. Open a terminal and navigate to the directory containing the `Cell_Balancing.c` file.
2. Compile the code using the following command:
   ```bash
   gcc -o Cell_Balancing Cell_Balancing.c
   ```
3. Make the executable file runnable (if required):
   ```bash
   chmod +x Cell_Balancing
   ```
4. Run the executable:
   ```bash
   ./Cell_Balancing
   ```
5. Observe the output printed in the terminal. It will display the cell voltages, their balancing status, and the reason for the status.