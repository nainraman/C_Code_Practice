# Live Peak Detector

This document explains the functionality of the `Live Peak Detector` program, which is implemented in C. The program is designed to detect spikes in a stream of sensor data in real-time. Below, we provide an overview of the code, its components, and how it works.

---

## Overview

The `Live Peak Detector` program processes sensor data to detect spikes based on a baseline and thresholds. It uses an Exponential Moving Average (EMA) to update the baseline dynamically and identifies spikes when the difference between the current sample and the baseline exceeds a high threshold.

### Key Components

1. **Sensor Data Simulation**:
   - The `getSensorData()` function simulates sensor data, returning a baseline value of `1` and a spike value of `10` every 100 samples.

2. **Spike Detection Logic**:
   - The `Spike_Detector` function updates the baseline using EMA and detects spikes based on high and low thresholds.

3. **Real-Time Processing**:
   - The `run10ms` function is called every 10 milliseconds to process a new sample and check for spikes.

4. **Initialization**:
   - The `Spike_DetectorInit` function initializes the state of the detector with the first sample.

---

## Code Flow Explanation

The program follows these steps:

1. **Initialization**:
   - The `SpikeState` structure is initialized with the first sensor reading.

2. **Sensor Data Simulation**:
   - The `getSensorData()` function generates a stream of data, simulating spikes every 100 samples.

3. **Spike Detection**:
   - The `Spike_Detector` function updates the baseline and checks if the current sample exceeds the high threshold.
   - If a spike is detected, it waits for the signal to drop below the low threshold before resetting.

4. **Output**:
   - The program prints the sample, baseline, difference, and spike count for debugging.
   - If a spike is detected, it prints "Spike detected!".

---

## Important Notes

### 1. When to Use `st->` and When to Use `st.`
- **Use `st->`**: When `st` is a pointer to a structure. The `->` operator accesses members of the structure through the pointer.
- **Use `st.`**: When `st` is a direct instance of a structure. The `.` operator accesses members directly.

### 2. Purpose of `fflush(stdout)`
- `fflush(stdout)` ensures that output is immediately written to the terminal. Without it, output may be buffered and not appear until the buffer is full or the program ends.

---

## How to Run the Program

1. Compile the program:
   ```bash
   gcc -o Peak_Detector Peak_Detection.c -lm
   ```

2. Run the executable:
   ```bash
   ./Peak_Detector
   ```

3. Observe the output in the terminal. Spikes will be detected and printed as "Spike detected!".

---

## Example Output

```
Sample: 1.00, Baseline: 1.00, Diff: 0.00, SpikeCount: 0
Sample: 10.00, Baseline: 1.18, Diff: 8.82, SpikeCount: 1
Spike detected!
...
```

---

## Future Improvements

- Add support for real sensor data.
- Implement logging to a file for long-term analysis.
- Optimize the spike detection thresholds for specific use cases.