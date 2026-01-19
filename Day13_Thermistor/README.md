# NTC Thermistor Temperature Conversion

## Topic: Real-world Sensor Conversion (ADC → Volts → Ohms → Temp)

### The Scenario
You are reading a temperature sensor (NTC Thermistor) inside a battery pack. This is a common application in automotive systems, battery management systems, and IoT devices where precise temperature monitoring is critical for safety and performance.

### Hardware Setup
**A simple Voltage Divider Circuit:**
- 5V Supply connected to a 10kΩ Pull-up Resistor
- The connection point goes to the ADC and the Thermistor
- The Thermistor connects to Ground

```
5V ----[10kΩ Pullup]----+----[ADC Input]
                         |
                    [NTC Thermistor]
                         |
                       GND
```

### Physics Behind the Circuit
As Temperature increases, NTC (Negative Temperature Coefficient) Resistance decreases, so the voltage at the ADC input drops. This creates an inverse relationship between temperature and measured voltage.

### The Math (Chain of Conversion)

#### 1. ADC to Voltage:
$$V_{out} = \frac{ADC}{4095} \times 5.0V$$

Where:
- ADC = 12-bit ADC reading (0-4095)
- 5.0V = Reference voltage

#### 2. Voltage to Resistance (R_ntc):
Using the voltage divider rule ($V_{out} = V_{in} \times \frac{R_{ntc}}{R_{pullup} + R_{ntc}}$), we rearrange to solve for $R_{ntc}$:

$$R_{ntc} = R_{pullup} \times \frac{V_{out}}{5.0V - V_{out}}$$

#### 3. Resistance to Temperature (Beta Equation):
$$T_{kelvin} = \frac{1}{\frac{1}{T_{25}} + \frac{1}{\beta} \ln(\frac{R_{ntc}}{R_{25}})}$$

$$T_{celsius} = T_{kelvin} - 273.15$$

Where:
- $T_{25}$ = 298.15K (25°C reference temperature)
- $R_{25}$ = 10kΩ (resistance at 25°C)
- $\beta$ = 3435 (thermistor beta coefficient from datasheet)

## Code Implementation

The program implements the `GetTemp_C()` function that:

1. **Performs Safety Checks**: Detects open circuits (ADC = 4095) and short circuits (ADC = 0)
2. **Converts ADC to Voltage**: Using the 12-bit ADC scale and 5V reference
3. **Calculates Thermistor Resistance**: Using voltage divider mathematics
4. **Converts to Temperature**: Using the NTC beta equation with natural logarithm
5. **Returns Celsius**: Final conversion from Kelvin to Celsius

### Fault Handling
The function handles critical fault conditions:
- **Short to Ground** (ADC = 0): Returns -999.0°C
- **Open Circuit/Short to VRef** (ADC = 4095): Returns -999.0°C

Without these checks, the resistance calculation would divide by zero, causing undefined behavior.

## How to Generate the Executable and Run It

### Prerequisites
- GCC compiler installed on your system
- Standard C library with math.h support

### Step 1: Compile the Program
```bash
cd /Users/ramannain/Desktop/C_Code_Practice/Day13_Thermistor
gcc -o thermistor_program Thermistor.c -lm
```

**Important Notes:**
- The `-lm` flag links the math library (required for `logf()` function)
- The output executable will be named `thermistor_program`

### Step 2: Make the File Executable (if needed)
```bash
chmod +x thermistor_program
```

### Step 3: Run the Program
```bash
./thermistor_program
```

## Expected Output

When you run the program, you should see:

```
ADC Counts | Temp (C) | Expected Note
-----------|----------|--------------
      0    |  -999.0  | Fault Detected
   4095    |  -999.0  | Fault Detected
   2048    |    25.0  | Approx 25C (Room Temp)
   1000    |    57.4  | Hotter (Lower Resistance)
   3000    |     1.0  | Colder (Higher Resistance)
```

### Understanding the Results

1. **ADC 0 & 4095**: Fault conditions properly detected
2. **ADC 2048**: Mid-range reading gives 25°C (room temperature baseline)
3. **ADC 1000**: Lower voltage → Lower resistance → Higher temperature (57.4°C)
4. **ADC 3000**: Higher voltage → Higher resistance → Lower temperature (1.0°C)

## Real-World Applications

This type of temperature sensing is crucial in:
- **Battery Management Systems**: Preventing thermal runaway
- **Automotive Electronics**: Engine and battery temperature monitoring
- **HVAC Systems**: Room and equipment temperature control
- **Industrial Sensors**: Process temperature monitoring

## Key Learning Points

1. **Chain of Conversions**: ADC → Voltage → Resistance → Temperature
2. **Fault Detection**: Critical for safety in real-world applications
3. **Inverse Relationship**: Higher temperature = Lower resistance = Lower voltage for NTC thermistors
4. **Mathematical Implementation**: Using logarithmic equations for accurate temperature calculation