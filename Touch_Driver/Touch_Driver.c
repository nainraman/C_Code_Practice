#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Simulated "Registers" inside the Touch Chip (Memory Map)
// Index 0x00 to 0x0F
uint8_t virtual_touch_registers[16];

// --- Mock Hardware Interface ---
// Reads 'length' bytes starting from 'reg_addr' into 'buffer'
void I2C_Read(uint8_t reg_addr, uint8_t *buffer, uint8_t length) {
    for (int i = 0; i < length; i++) {
        if (reg_addr + i < 16) {
            buffer[i] = virtual_touch_registers[reg_addr + i];
        }
    }
}

// --- Driver Logic (Reverse Engineer This) ---
typedef struct {
    uint16_t x;
    uint16_t y;
    uint8_t event_flag; // 00=Down, 01=Up, 10=Contact
} TouchPoint;

bool Get_Touch_Coordinates(TouchPoint *point) {
    uint8_t raw_data[6]; // To store Reg 0x02 to 0x07 (Status, XH, XL, YH, YL...)
    
    // 1. Read 5 bytes starting from Register 0x02
    // Reg 0x02: Status (Number of points)
    // Reg 0x03: X High (With Event Flags!)
    // Reg 0x04: X Low
    // Reg 0x05: Y High (ID + Upper bits)
    // Reg 0x06: Y Low
    I2C_Read(0x02, raw_data, 5);
    
    uint8_t num_points = raw_data[0]; // Reg 0x02
    
    // TODO: If num_points is 0, return false (No touch).
    if (num_points == 0) {
        return false; // No touch detected
    }
    
    // Reverse Engineering Task:
    // Extract X and Y.
    // CAUTION: Reg 0x03 contains Flags in bits [7:6].
    // Format: [Flag(2) | Reserved(2) | X_High(4)]
    
    uint8_t x_high_byte = raw_data[1]; // Reg 0x03
    uint8_t x_low_byte  = raw_data[2]; // Reg 0x04
    uint8_t y_high_byte = raw_data[3]; // Reg 0x05
    uint8_t y_low_byte  = raw_data[4]; // Reg 0x06
    
    // TODO: Extract the Event Flag (Top 2 bits of x_high_byte)
    // point->event_flag = ...
    point->event_flag = (x_high_byte >> 6) & 0x03; // Extract bits 7 and 6
    
    // TODO: Clean the High Bytes (Mask out the flags!)
    // Hint: We only want the bottom 4 bits of the High Byte.
    x_high_byte = x_high_byte & 0x0F; // Mask to keep only lower 4 bits
    y_high_byte = y_high_byte & 0x0F; // Mask to keep only lower 4 bits
    
    // TODO: Combine High and Low to form 16-bit X and Y
    // point->x = ...
    // point->y = ...
    point->x = ((uint16_t)x_high_byte << 8) | x_low_byte;
    point->y = ((uint16_t)y_high_byte << 8) | y_low_byte;
    
    return true; 
}

// --- Test Harness ---
int main() {
    TouchPoint pt;
    
    // Scenario 1: Clean Touch
    // X = 0x0150 (336), Y = 0x0080 (128)
    // No Event Flags (00) -> X_High = 0x01
    printf("--- Test 1: Clean Touch ---\n");
    virtual_touch_registers[0x02] = 1;    // 1 Touch point
    virtual_touch_registers[0x03] = 0x01; // X High (00 00 0001)
    virtual_touch_registers[0x04] = 0x50; // X Low
    virtual_touch_registers[0x05] = 0x00; // Y High
    virtual_touch_registers[0x06] = 0x80; // Y Low
    
    if (Get_Touch_Coordinates(&pt)) {
        printf("X: %d (Expect 336)\n", pt.x);
        printf("Y: %d (Expect 128)\n", pt.y);
    }

    // Scenario 2: The "Glitch" (Event Flag is Set)
    // Same position (336), but Event Flag = "Contact" (10 -> Binary 10xxxxxx)
    // X_High becomes 0x81 (1000 0001) instead of 0x01
    printf("\n--- Test 2: Touch with Event Flag (The Bug) ---\n");
    virtual_touch_registers[0x02] = 1;
    virtual_touch_registers[0x03] = 0x81; // <--- The Trap! 0x80 bit is set
    virtual_touch_registers[0x04] = 0x50; 
    virtual_touch_registers[0x05] = 0x00; 
    virtual_touch_registers[0x06] = 0x80;

    if (Get_Touch_Coordinates(&pt)) {
        printf("Event Flag: %d (Expect 2 for 'Contact')\n", pt.event_flag);
        printf("X: %d (Expect 336. If 33104, you failed masking)\n", pt.x);
        printf("Y: %d (Expect 128)\n", pt.y);
    }

    return 0;
}