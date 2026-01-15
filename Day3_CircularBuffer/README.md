# Circular Buffer Implementation

## Topic: Efficient Data History & "Windowed" Calculations

### The Scenario
You are sampling a High Voltage Battery cell every 10ms. The ADC is noisy. To get a stable reading for the BMS logic, you need the average of the last 8 samples.

### The Naive (Bad) Way
Store samples in an array. Every new sample, shift everything down (e.g., `data[0] = data[1]`, etc.) to make room.

#### Why it fails:
- Using `memmove` or looping to shift data is **O(N)**.
- If `N` is large, you waste CPU cycles doing busy work.

### The Embedded Way (Ring Buffer)
You use a fixed array and a "head" index. When the head reaches the end, it wraps back to 0. You never shift data; you just overwrite the oldest value.

#### Complexity:
- **Insertion** is **O(1)**.

### Your Goal
1. **Implement AddSample**: Add a new voltage to the buffer. Handle the "wrap around" logic.
2. **Implement GetAverage**: Calculate the average of valid samples.
   - Crucial: If you have only collected 3 samples so far, the average should be `sum / 3`, not `sum / 8`.

---

## Code Explanation

### 1. **RingBuffer Structure**
The `RingBuffer` structure is defined as:
```c
typedef struct {
    float buffer[BUFFER_SIZE];
    uint8_t head;  // Index where the NEXT sample will be written
    uint8_t count; // How many valid samples we have (max BUFFER_SIZE)
} RingBuffer;
```
- `buffer`: Fixed-size array to store the samples.
- `head`: Tracks the index where the next sample will be written.
- `count`: Tracks the number of valid samples in the buffer.

### 2. **RB_Init**
Initializes the `RingBuffer`:
- Sets `head` and `count` to 0.
- Fills the buffer with 0.0.

### 3. **RB_AddSample**
Adds a new sample to the buffer:
- Writes the sample at the `head` index.
- Increments `head` with wrap-around logic using `& BUFFER_MASK`.
- Increments `count` but clamps it at `BUFFER_SIZE`.

### 4. **RB_GetAverage**
Calculates the average of valid samples:
- If `count` is 0, returns 0.0 to avoid divide-by-zero.
- Sums up the valid elements in the buffer and divides by `count`.

### 5. **Main Function**
- **Test Case 1**: Adds 3 samples and calculates the average.
- **Test Case 2**: Adds 6 more samples, demonstrating wrap-around logic, and calculates the average.

---

## How to Compile and Run

### 1. Open Terminal
Navigate to the directory containing the `Cicular_Buffer.c` file:
```bash
cd /Users/ramannain/Desktop/C_Code_Practice/Circular_Buffer
```

### 2. Compile the Code
Use the `gcc` compiler to generate an executable:
```bash
gcc -o Circular_Buffer Cicular_Buffer.c
```
- `-o Circular_Buffer`: Specifies the name of the output executable.
- `Cicular_Buffer.c`: The name of the source file.

### 3. Run the Executable
Run the generated executable:
```bash
./Circular_Buffer
```

### 4. Verify Output
Check the terminal for the program's output. It should display the results of the test cases:
- Average after 3 samples: **11.00**
- Average after wrap-around: **14.50**

---

## Summary
This program demonstrates an efficient way to manage a fixed-size buffer for real-time data processing. The ring buffer ensures constant time complexity for insertion and avoids unnecessary memory operations.