# 1D Interpolation: OCV to SOC Mapping

## Topic: Non-Linear Mapping (OCV to SOC)

### The Scenario
You are writing the State-of-Charge (SOC) estimator. You cannot measure SOC directly; you measure Voltage. However, Lithium Iron Phosphate (LFP) batteries—like those used in standard range Teslas—have a very "flat" voltage curve.

- **Example:**
  - 3.20V might be 30% SOC.
  - 3.25V might be 60% SOC.

A simple linear equation (`y = mx + c`) is not accurate enough.

### The Solution
We use a **Look-Up Table (LUT)** with interpolation. We define known points and draw straight lines between them.

---

## Code Explanation

### 1. **Look-Up Table (LUT)**
The LUT is defined as two arrays:
- `LUT_Voltage`: Known voltage values (X-axis).
- `LUT_SOC`: Corresponding SOC values (Y-axis).

These arrays must be strictly increasing for the interpolation logic to work.

### 2. **GetSOC_From_Voltage Function**
This function maps a given voltage to the corresponding SOC using the LUT.

#### Steps:
1. **Handle Bounds:**
   - If the voltage is below the lowest point in the table, return `0.0%`.
   - If the voltage is above the highest point, return `100.0%`.

2. **Find the Interval:**
   - Loop through the LUT to find which two points `[i]` and `[i+1]` bracket the input voltage.

3. **Interpolate:**
   - Use the point-slope formula to calculate the SOC between the two points:
     ```c
     SOC = soc_low + (voltage - v_low) * ((soc_high - soc_low) / (v_high - v_low))
     ```

### 3. **Main Function**
The `main` function tests the interpolation logic with various voltages:
- **Test Cases:**
  - 2.9V -> Clamped to `0.0%`.
  - 3.0V -> Exact match with the first LUT entry.
  - 3.1V -> Interpolated between 3.0V and 3.2V.
  - 3.225V -> Interpolated between 3.2V and 3.25V.
  - 3.6V -> Exact match with the last LUT entry.
  - 3.7V -> Clamped to `100.0%`.

---

## How to Compile and Run

### 1. Open Terminal
Navigate to the directory containing the `1D_Interpolation.c` file:
```bash
cd /Users/ramannain/Desktop/C_Code_Practice/Day4_1DInterpolation
```

### 2. Compile the Code
Use the `gcc` compiler to generate an executable:
```bash
gcc -o 1D_Interpolation 1D_Interpolation.c
```
- `-o 1D_Interpolation`: Specifies the name of the output executable.
- `1D_Interpolation.c`: The name of the source file.

### 3. Run the Executable
Run the generated executable:
```bash
./1D_Interpolation
```

### 4. Verify Output
Check the terminal for the program's output. It should display the results of the test cases:
- Voltage: 2.900 V -> SOC: 0.00%
- Voltage: 3.000 V -> SOC: 0.00%
- Voltage: 3.100 V -> SOC: 15.00%
- Voltage: 3.225 V -> SOC: 45.00%
- Voltage: 3.600 V -> SOC: 100.00%
- Voltage: 3.700 V -> SOC: 100.00%

---

## Summary
This program demonstrates how to use a Look-Up Table (LUT) with interpolation to map non-linear relationships efficiently. The bounds handling and interpolation logic ensure accurate SOC estimation for a given voltage input.