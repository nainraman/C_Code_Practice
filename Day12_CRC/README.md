# Ensuring the message wasn't corrupted by noise

## The Scenario
The Battery Management System (BMS) sends a message: Voltage = 400V. However, electrical noise (EMI) flips a bit during transmission. The receiver reads Voltage = 656V. If the receiver acts on this incorrect value, it might trigger a false panic. To prevent this, we append a "fingerprint" (CRC) at the end of every message.

## The Math (Simplified)

### Checksum (XOR)
- **Fast but weak**: Detecting double-bit errors is poor.

### CRC (Cyclic Redundancy Check)
- **Gold standard in CAN/Automotive**: Uses polynomial division.
- **How it works**:
  - Treat the data as a massive number.
  - "Divide" it by a specific polynomial (e.g., 0x1D).
  - The "Remainder" is the CRC byte.
  - If the receiver does the math and gets a remainder of 0, the data is valid.

## Your Goal

### Task 1 (Warm-up): Simple XOR Checksum
- **Logic**: checksum = byte1 ^ byte2 ^ byte3 ...

### Task 2 (The Real Deal): CRC-8 (SAE J1850 Standard)
- **Polynomial**: 0x1D (x^8 + x^4 + x^3 + x^2 + 1).
- **Algorithm**:
  1. For each byte: XOR it into the CRC register.
  2. For each bit (8 times):
     - If MSB is 1 (0x80): Shift Left, then XOR with Poly 0x1D.
     - Else: Just Shift Left.

## How to Generate and Run the Executable

1. **Compile the C File**:
   ```bash
   gcc /Users/ramannain/Desktop/C_Code_Practice/Day12_CRC/CRC_Safety.c -o /Users/ramannain/Desktop/C_Code_Practice/Day12_CRC/CRC_Safety
   ```

2. **Make the File Executable**:
   ```bash
   chmod +x /Users/ramannain/Desktop/C_Code_Practice/Day12_CRC/CRC_Safety
   ```

3. **Run the Executable**:
   ```bash
   /Users/ramannain/Desktop/C_Code_Practice/Day12_CRC/CRC_Safety
   ```

## Observed Results

### Task 1: XOR Checksum
- **Output**: `XOR Checksum: 0x70`
- **Expected**: `0x70`
- **Conclusion**: XOR checksum calculation is correct.

### Task 2: CRC-8 (SAE J1850)
- **Output**: `CRC-8 Result: 0x67`
- **Expected**: `0x2F`
- **Conclusion**: There is a mismatch. Verify the polynomial or initial value.

### Test 3: Bit Flip Detection
- **Output**: `SUCCESS: Bit flip detected! New CRC: 0x2B`
- **Conclusion**: The CRC algorithm successfully detected a bit flip.