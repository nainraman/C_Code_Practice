# Fault Manager Documentation

## Overview
This document provides an explanation of the `Fault_Manager.c` script, including the use of the `#define` preprocessor directive and bitwise operators. The script simulates a fault management system for a device, where faults are represented as bit flags.

---

## Code Structure

### 1. Fault Definitions
The script uses the `#define` preprocessor directive to define fault flags as bit masks:

```c
#define FAULT_OVER_VOLTAGE   (1 << 0) // 0x01
#define FAULT_UNDER_VOLTAGE  (1 << 1) // 0x02
#define FAULT_OVER_TEMP      (1 << 2) // 0x04
#define FAULT_COMM_LOSS      (1 << 3) // 0x08
```

Each fault is represented as a single bit in a 32-bit integer. The `1 << n` operation shifts the bit `1` to the left by `n` positions, creating unique values for each fault.

### 2. Critical Faults
Critical faults are defined using a bitwise OR operation:

```c
#define CRITICAL_MASK (FAULT_OVER_VOLTAGE | FAULT_UNDER_VOLTAGE | FAULT_OVER_TEMP)
```

This creates a mask that combines the `FAULT_OVER_VOLTAGE`, `FAULT_UNDER_VOLTAGE`, and `FAULT_OVER_TEMP` flags. If any of these faults are active, the contactors should open.

### 3. Fault Manager Functions
- **Initialization**: Resets all faults.
- **SetFault**: Activates a specific fault.
- **ClearFault**: Deactivates a specific fault.
- **IsFaultActive**: Checks if a specific fault is active.
- **ShouldOpenContactors**: Determines if the contactors should open based on critical faults.

---

## Notes on `#define` Preprocessor Directive
The `#define` directive is used to create macros, which are text replacements performed by the preprocessor before compilation. In this script, it is used to define fault flags and the critical fault mask.

### Example:
```c
#define FAULT_OVER_VOLTAGE   (1 << 0)
```
This defines `FAULT_OVER_VOLTAGE` as `0x01` (binary `0001`).

---

## Notes on Bitwise Operators
Bitwise operators perform operations on individual bits of integers. They are used extensively in this script to manage fault flags.

### Common Operators:
1. **Bitwise OR (`|`)**: Combines bits. Used to set multiple flags.
   - Example: `a | b`
2. **Bitwise AND (`&`)**: Checks if specific bits are set.
   - Example: `a & b`
3. **Bitwise NOT (`~`)**: Inverts bits. Used to clear specific flags.
   - Example: `~a`
4. **Bitwise Shift (`<<`, `>>`)**: Shifts bits left or right.
   - Example: `1 << 2` results in `0x04` (binary `0100`).

### Implementation in the Script:
- **Set a Fault**:
  ```c
  fm->active_faults |= FAULT_OVER_VOLTAGE;
  ```
  This sets the `FAULT_OVER_VOLTAGE` bit without affecting other bits.

- **Clear a Fault**:
  ```c
  fm->active_faults &= ~FAULT_OVER_VOLTAGE;
  ```
  This clears the `FAULT_OVER_VOLTAGE` bit while leaving other bits unchanged.

- **Check a Fault**:
  ```c
  if (fm->active_faults & FAULT_OVER_VOLTAGE)
  ```
  This checks if the `FAULT_OVER_VOLTAGE` bit is set.

---

## Simulation Test
The `main` function simulates two scenarios:
1. **Setting a Critical Fault**:
   - Activates `FAULT_OVER_VOLTAGE`.
   - Verifies that the contactors open.

2. **Setting a Non-Critical Fault**:
   - Activates `FAULT_COMM_LOSS`.
   - Clears the critical fault.
   - Verifies that the contactors remain closed.

---

## Conclusion
This script demonstrates the use of bitwise operators and the `#define` directive to manage fault flags efficiently. The modular design allows for easy extension to additional faults or logic.