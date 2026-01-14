#include <stdio.h>
#include <stdint.h>

// Simulated CAN Message (8 bytes)
// scenario: Current = -500 (0xFE0C in hex), Voltage = 40000 (0x9C40 in hex)
// Current is Big Endian: [0xFE, 0x0C]
// Voltage is Little Endian: [0x40, 0x9C] (Low byte first)
uint8_t rx_data[8] = {0xFE, 0x0C, 0x40, 0x9C, 0x00, 0x00, 0x00, 0x00};

// Task 1: Bitwise Shifting (Safe & Portable)
void Parse_Can_Safe(uint8_t *data, int16_t *current, uint16_t *voltage) {
    // TODO: Parse Current (Big Endian)
    // Hint: High byte is at index 0. Shift it left by 8, OR with index 1.
    // Cast to int16_t to ensure sign is preserved.
    *current = (int16_t)((data[0] << 8) | data[1]);
    
    // TODO: Parse Voltage (Little Endian)
    // Hint: Low byte is at index 2. High byte is at index 3.
    *voltage = (uint16_t)(data[2] | (data[3] << 8));
}

// Task 2: The "Union" Method (Dangerous but fast)
// Implement this to understand how memory overlay works.
void Parse_Can_Union(uint8_t *data) {
    typedef union {
        uint8_t bytes[2];
        uint16_t value;
    } Parser;
    
    // TODO: Use this union to parse bytes [2] and [3] (Voltage)
    // 1. Assign bytes to the union array
    // 2. Print the .value member
    Parser voltage_parser;
    
    voltage_parser.bytes[0] = data[2]; // Low byte
    voltage_parser.bytes[1] = data[3]; // High byte
    uint16_t voltage = voltage_parser.value;
    printf("Union Parsed Voltage: 0x%04X (Expected: 0x9C40)\n", voltage);
    // Question: Why might this fail if we moved this code to a Big Endian processor?

}

int main() {
    int16_t current_raw = 0;
    uint16_t voltage_raw = 0;

    printf("--- Safe Parsing (Bitwise) ---\n");
    Parse_Can_Safe(rx_data, &current_raw, &voltage_raw);

    // Apply scaling (Physically meaningful values)
    float current_phys = current_raw * 0.1f;
    float voltage_phys = voltage_raw * 0.01f;

    printf("Raw Hex Current: 0x%04X (Expected: 0xFE0C)\n", (uint16_t)current_raw);
    printf("Physical Current: %.1f A (Expected: -50.0 A)\n", current_phys);
    
    printf("Raw Hex Voltage: 0x%04X (Expected: 0x9C40)\n", voltage_raw);
    printf("Physical Voltage: %.2f V (Expected: 400.00 V)\n", voltage_phys);

    printf("\n--- Unsafe Parsing (Union) ---\n");
    Parse_Can_Union(rx_data);

    return 0;
}