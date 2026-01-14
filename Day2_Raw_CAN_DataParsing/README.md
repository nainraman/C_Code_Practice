# Understanding Endianness and Union Behavior

This document explains the concept of **endianness** and how it affects the behavior of unions in the context of the `Parse_Can_Union` function in the `CAN_DataParsing.c` file. This is important to understand why the union method might fail on a Big Endian processor.

---

## What is Endianness?
Endianness refers to the order in which bytes are stored in memory for multi-byte data types (e.g., `uint16_t`, `int32_t`).

- **Little Endian**: The least significant byte (LSB) is stored at the lowest memory address, and the most significant byte (MSB) is stored at the highest memory address.
- **Big Endian**: The most significant byte (MSB) is stored at the lowest memory address, and the least significant byte (LSB) is stored at the highest memory address.

### Example: Storing `0x9C40` (16-bit value)
| Endianness      | Memory Address 0 | Memory Address 1 |
|-----------------|------------------|------------------|
| Little Endian  | `0x40`           | `0x9C`           |
| Big Endian     | `0x9C`           | `0x40`           |

---

## How the Union Works
In the `Parse_Can_Union` function, the following union is used:

```c
typedef union {
    uint8_t bytes[2];
    uint16_t value;
} Parser;
```

- The `bytes` array and the `value` field share the same memory.
- When you modify `bytes[0]` and `bytes[1]`, the `value` field is automatically updated to reflect the combined interpretation of those two bytes as a `uint16_t`.

### Code Example
```c
Parser voltage_parser;
voltage_parser.bytes[0] = data[2]; // Low byte
voltage_parser.bytes[1] = data[3]; // High byte
uint16_t voltage = voltage_parser.value;
```

- `voltage_parser.bytes[0]` is assigned `data[2]` (0x40).
- `voltage_parser.bytes[1]` is assigned `data[3]` (0x9C).
- The `value` field interprets these bytes as a `uint16_t`.

---

## Why the Union Fails on Big Endian
The issue arises because the `value` field's interpretation depends on the system's endianness:

- On a **Little Endian system**:
  - `bytes[0]` is interpreted as the LSB.
  - `bytes[1]` is interpreted as the MSB.
  - The `value` becomes `0x9C40` (correct).

- On a **Big Endian system**:
  - `bytes[0]` is interpreted as the MSB.
  - `bytes[1]` is interpreted as the LSB.
  - The `value` becomes `0x409C` (incorrect).

### Key Takeaway
The union method relies on the system's memory layout for multi-byte values, which is affected by endianness. This makes the union method non-portable across systems with different endianness.

---

## Why Bitwise Operations Are Safe
In the `Parse_Can_Safe` function, the voltage is parsed using bitwise operations:

```c
*voltage = (uint16_t)(data[2] | (data[3] << 8));
```

- `data[2]` (low byte) is placed in the lower 8 bits.
- `data[3]` (high byte) is shifted left by 8 bits and placed in the upper 8 bits.

This operation is **explicit** and does not depend on the system's endianness. The result will always be:

```
*voltage = 0x40 | (0x9C << 8)
         = 0x40 | 0x9C00
         = 0x9C40
```

### Key Takeaway
Bitwise operations are safe and portable because they explicitly define how the bytes are combined, independent of the system's endianness.

---

## Conclusion
- The `rx_data` array does not change based on endianness. `data[2]` is always `0x40`, and `data[3]` is always `0x9C`.
- The union method fails on Big Endian systems because it relies on the system's memory layout for multi-byte values.
- Bitwise operations are safe and portable because they explicitly define how the bytes are combined.

Understanding these concepts is crucial for writing portable and reliable code when working with multi-byte data in embedded systems or cross-platform applications.