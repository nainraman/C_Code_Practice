#include <stdio.h>
#include <stdint.h>

// Standard SAE J1850 Polynomial: x^8 + x^4 + x^3 + x^2 + 1
// Binary: 0001 1101 -> 0x1D
#define CRC_POLY 0x1D

// Task 1: Simple XOR Checksum (Weak but fast)
uint8_t Calculate_XOR_Checksum(const uint8_t *data, size_t length) {
    uint8_t checksum = 0;
    // TODO: Iterate through data and XOR each byte into checksum
    for (size_t i = 0; i < length; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

// Task 2: Robust CRC-8 Algorithm (Bit-wise calculation)
// This is the "interview version". In production, we'd use a Lookup Table for speed.
uint8_t Calculate_CRC8(const uint8_t *data, size_t length) {
    uint8_t crc = 0x00; // Initial value (sometimes 0xFF in other standards)

    for (size_t i = 0; i < length; i++) {
        // 1. XOR input byte into the CRC register
        crc ^= data[i];

        // 2. Process all 8 bits
        for (int bit = 0; bit < 8; bit++) {
            // TODO: Check if MSB (0x80) is set
            if ((crc & 0x80) != 0) {
                // If 1: Shift left AND XOR with Polynomial
                // Note: Shift first, then XOR? Or XOR then shift?
                // Standard logic: Shift first (discarding MSB), then XOR.
                // But wait! If we XOR the Poly, we are "subtracting" it.
                // Let's stick to the standard logic:
                // crc = (crc << 1) ^ CRC_POLY;
                crc = (crc << 1) ^ CRC_POLY;
            } else {
                // If 0: Just Shift left
                // crc = crc << 1;
                crc = crc << 1;
            }
        }
    }
    return crc;
}

int main() {
    // Test Data: { 0x12, 0x34, 0x56 }
    uint8_t data[] = { 0x12, 0x34, 0x56 };
    size_t len = sizeof(data);

    printf("--- Task 1: XOR Checksum ---\n");
    // 0x12 ^ 0x34 ^ 0x56 = 0x70
    uint8_t xor_sum = Calculate_XOR_Checksum(data, len);
    printf("XOR Checksum: 0x%02X (Expect 0x70)\n", xor_sum);

    printf("\n--- Task 2: CRC-8 (SAE J1850) ---\n");
    // Manual calculation check for {0x12, 0x34, 0x56}:
    // Result should be 0x2F (if using Init=0x00, Poly=0x1D)
    uint8_t crc = Calculate_CRC8(data, len);
    printf("CRC-8 Result: 0x%02X (Expect 0x2F)\n", crc);
    
    // Verification: What happens if a bit flips?
    printf("\n--- Test 3: Bit Flip Detection ---\n");
    data[1] = 0x35; // Flip one bit in second byte (0x34 -> 0x35)
    uint8_t bad_crc = Calculate_CRC8(data, len);
    
    if (bad_crc != crc) {
        printf("SUCCESS: Bit flip detected! New CRC: 0x%02X\n", bad_crc);
    } else {
        printf("FAILURE: CRC Collision (Rare but bad).\n");
    }

    return 0;
}