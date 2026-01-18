# Defensive Programming & Fault Isolation

## Overview
This document explains the purpose and functionality of the `Plausibility_Check.c` file, which implements a defensive programming approach to sanitize raw current sensor readings. It also provides instructions to compile and run the program.

### Topic: Defensive Programming & Fault Isolation

#### The Scenario
You are reading the main current sensor (e.g., a Shunt or Hall Effect sensor). Hardware is imperfect, and the following issues may arise:

1. **ADC Glitch**: A cosmic ray or EMI spike might cause the ADC to read NaN (Not a Number) or Infinity.
2. **Physics Violation**: The current cannot jump from 0A to 500A in 10ms (due to inductance). This is a Slew Rate violation.
3. **Range Violation**: The fuse blows at 1000A. If you read 2000A, the sensor is likely broken (short to rail).

#### Your Goal
Write a "wrapper" function that sanitizes the raw input:

1. **NaN/Inf Check**: Reject the sample immediately if it's not a valid number.
2. **Range Check**: Reject if outside `[-1000.0, 1000.0]`.
3. **Slew Rate Check**: Reject if the value changes by more than 100A since the last valid sample.

#### Behavior
- **If Valid**: Update the `last_valid_current` and write to the output pointer. Return `true`.
- **If Invalid**: Do not update the output pointer (hold the last good value). Return `false`.

### Functionality
The program implements the `GetSafeCurrent` function, which:
- Checks for invalid values (NaN or Infinity).
- Ensures the reading is within the acceptable range of `[-1000.0, 1000.0]`.
- Verifies that the change in current does not exceed the maximum allowed slew rate of 100A.

If all checks pass, the function updates the last valid current value and returns `true`. Otherwise, it rejects the input and returns `false`.

### Test Cases
The program tests the function with the following inputs:
1. `10.0` A: Accepted.
2. `50.0` A: Accepted (jump of 40A is OK).
3. `2000.0` A: Rejected (out of range).
4. `NaN`: Rejected (invalid number).
5. `200.0` A: Rejected (slew rate violation).
6. `60.0` A: Accepted (jump of 10A is OK).

### Results
The program outputs the following results:
```
Input    | Accepted? | Output (Clean)
---------|-----------|---------------
   10.0  |     1     |    10.0
   50.0  |     1     |    50.0
 2000.0  |     0     |    50.0
    nan  |     0     |    50.0
  200.0  |     0     |    50.0
   60.0  |     1     |    60.0
```

## Instructions

### Compilation
To compile the program, run the following command in the terminal:
```bash
gcc -o Plausibility_Check Plausibility_Check.c
```

### Make Executable
Make the compiled file executable:
```bash
chmod +x Plausibility_Check
```

### Execution
Run the program:
```bash
./Plausibility_Check
```

The program will display the results of the test cases in the terminal.