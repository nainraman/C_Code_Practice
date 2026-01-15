#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Define buffer size (Power of 2 for optimization)
#define BUFFER_SIZE 8
#define BUFFER_MASK (BUFFER_SIZE - 1) // 7 (binary 00000111)

typedef struct {
    float buffer[BUFFER_SIZE];
    uint8_t head;  // Index where the NEXT sample will be written
    uint8_t count; // How many valid samples we have (max BUFFER_SIZE)
} RingBuffer;

// 1. Initialize
void RB_Init(RingBuffer *rb) {
    rb->head = 0;
    rb->count = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        rb->buffer[i] = 0.0f;
    }
}

// 2. Add a sample (O(1) complexity)
void RB_AddSample(RingBuffer *rb, float sample) {
    // TODO:
    rb->buffer[rb->head] = sample;
    rb->head = (rb->head + 1) & BUFFER_MASK; // Wrap around using bitwise AND
    if (rb->count < BUFFER_SIZE) {
        rb->count++;
    }
    // Steps:
    // 1. Store sample at 'rb->head'
    // 2. Increment 'rb->head' safely (Wrap around using bitwise AND or Modulo)
    // 3. Increment 'rb->count' but clamp it at BUFFER_SIZE (don't let it grow to infinity)
}

// 3. Calculate Average
float RB_GetAverage(RingBuffer *rb) {
    // TODO:
    // 1. If count is 0, return 0.0 to avoid divide-by-zero.
    // 2. Sum up the VALID elements (iterate 'rb->count' times, or just loop through whole buffer if full?)
    // Note: Since we are averaging *all* valid data in the buffer, a simple loop 0..count-1 works 
    // if the buffer isn't full. If it IS full, you sum all indices 0..7.
    if (rb->count == 0) {
        return 0.0f; // Avoid divide by zero
    }
    float sum = 0.0f;
    for (uint8_t i = 0; i < rb->count; i++) {
        sum += rb->buffer[i];
    }
    return sum / rb->count;
    
    return 0.0f; 
}

int main() {
    RingBuffer rb;
    RB_Init(&rb);

    // Test Case 1: Partially Full
    printf("--- Injecting 3 Samples ---\n");
    RB_AddSample(&rb, 10.0f);
    RB_AddSample(&rb, 11.0f);
    RB_AddSample(&rb, 12.0f);
    
    // Expect Average: 11.0
    printf("Average (Expect 11.0): %.2f\n", RB_GetAverage(&rb));

    // Test Case 2: Wrap Around
    printf("\n--- Injecting 6 More Samples (Wrap Around) ---\n");
    // Buffer was [10, 11, 12, 0, 0, 0, 0, 0] (head=3, count=3)
    // Add: 13, 14, 15, 16, 17, 18
    // Buffer should eventually look like: [18, 11, 12, 13, 14, 15, 16, 17] (head=1, count=8)
    // Note: 10 was overwritten by 18.
    
    RB_AddSample(&rb, 13.0f);
    RB_AddSample(&rb, 14.0f);
    RB_AddSample(&rb, 15.0f);
    RB_AddSample(&rb, 16.0f);
    RB_AddSample(&rb, 17.0f);
    RB_AddSample(&rb, 18.0f);

    float avg = RB_GetAverage(&rb);
    // Values in buffer: 11, 12, 13, 14, 15, 16, 17, 18
    // Sum = 116, Avg = 14.5
    printf("Average (Expect 14.5): %.2f\n", avg);
    
    return 0;
}