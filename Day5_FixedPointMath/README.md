# Precision without Floating Point

## The Scenario
Your BMS microcontroller (e.g., a smaller safety chip) does not have a Floating Point Unit (FPU). Using float causes the compiler to pull in a slow, heavy software library. You need to filter a noisy voltage signal using the same EMA filter from your Peak Detector, but using only integers.

## The Math
### Standard EMA:
$$Y_{new} = \alpha \times X + (1 - \alpha) \times Y_{old}$$

### The Fixed-Point Trick:
We scale everything up by a factor. A power of 2 (like 1024) is best because division becomes a bit-shift (>> 10).

- **Scaling Factor**: $1024$ ($2^{10}$)
- **Representing 1.0**: $1024$
- **Representing Alpha (0.1)**: $0.1 \times 1024 \approx 102$

### New Formula:
$$Y_{new} = \frac{102 \times X + (1024 - 102) \times Y_{old}}{1024}$$

## Implementation
The `Fixed_PointMath.c` file implements the above formula in two ways:
1. **Floating Point Version**: For reference, the `EMA_Update_Float` function uses standard floating-point arithmetic.
2. **Fixed Point Version**: The `EMA_Update_Fixed` function uses integer arithmetic with bit-shifting for division and rounding for precision.

### Key Features of Fixed Point Implementation:
- **Bit Shifting**: Division by 1024 is replaced with `>> 10`.
- **Rounding**: To minimize truncation errors, the numerator is adjusted by adding half the divisor before shifting: `(numerator + 512) >> 10`.

## How to Compile and Run

### Compilation
To compile the program, use the following command:
```bash
gcc -o Fixed_PointMath Fixed_PointMath.c
```
This will generate an executable named `Fixed_PointMath`.

### Execution
Run the executable using:
```bash
./Fixed_PointMath
```

### Output
The program compares the floating-point and fixed-point EMA implementations for a series of test inputs. It prints the results in a tabular format, showing the iteration, floating-point average, fixed-point average, and the error between the two.

### Example Output:
```
Iter | Float Avg | Fixed Avg | Error
-----|-----------|-----------|------
  1  |  3330.00  |   3330   | 0.00
  2  |  3363.00  |   3363   | 0.00
  3  |  3396.70  |   3397   | -0.30
  4  |  3427.03  |   3427   | 0.03
  5  |  3454.33  |   3454   | -0.33
```

### Notes
- Small errors (off by 1) are acceptable due to integer truncation.
- Large errors indicate a potential issue in the fixed-point math logic.

## Conclusion
This example demonstrates how to achieve precision without floating-point arithmetic, making it suitable for microcontrollers without an FPU.