# Day 14: Callback-Driven State Machine

## Topic: Scalable Architecture

### The Scenario
In complex automotive systems (like the Vehicle Control Unit or BMS), a giant switch-case statement with 50 states becomes unreadable and hard to maintain. Tesla and other top-tier companies often use **Function Pointers (Jump Tables)** to implement State Machines for better scalability and maintainability.

### Why Function Pointer State Machines?

**Traditional Switch-Case Problems:**
```c
switch(state) {
    case STATE_IDLE:
        // 50 lines of code
        break;
    case STATE_ACTIVE:
        // 100 lines of code
        break;
    // ... 48 more cases
}
```

**Function Pointer Solution:**
```c
StateTable[current_state]();  // One line execution!
```

## Function Pointer Syntax Patterns

### Basic Function Pointer typedef Syntax

```c
typedef return_type (*PointerTypeName)(parameter_types);
```

This creates a **custom type** that can hold pointers to functions matching that signature.

### Pattern Breakdown

```c
typedef void (*StateHandler)(void);
         ↑         ↑           ↑
    return type  type name  parameters
```

### Common Function Pointer Examples

#### Example 1: State Machine Handlers
```c
// Functions that return void and take no parameters
typedef void (*StateHandler)(void);

// Usage:
StateHandler current_handler = Handler_Idle;
current_handler();  // Calls Handler_Idle()
```

#### Example 2: Mathematical Operations
```c
// Functions that return int and take two int parameters
typedef int (*Calculator)(int a, int b);

// Implementation functions:
int Add(int a, int b) { return a + b; }
int Multiply(int a, int b) { return a * b; }

// Usage:
Calculator math_ops[] = { Add, Multiply };
int result = math_ops[0](5, 3);  // Calls Add(5, 3) = 8
```

#### Example 3: Event Callbacks
```c
// Functions that return void and take an event parameter
typedef void (*EventCallback)(int event_code);

// Implementation:
void OnButtonPress(int code) { 
    printf("Button %d pressed!\n", code); 
}
void OnSensorUpdate(int code) { 
    printf("Sensor %d updated!\n", code); 
}

// Usage:
EventCallback callbacks[] = { OnButtonPress, OnSensorUpdate };
callbacks[0](1);  // "Button 1 pressed!"
```

#### Example 4: String Processing
```c
// Functions that return char* and take one string parameter
typedef char* (*StringProcessor)(const char* input);

// Implementation:
char* ToUpper(const char* str) { /* convert to uppercase */ }
char* ToLower(const char* str) { /* convert to lowercase */ }

// Usage:
StringProcessor formatters[] = { ToUpper, ToLower };
char* result = formatters[0]("hello");  // Returns "HELLO"
```

#### Example 5: Comparison Functions
```c
// Functions that return int (for sorting) and take two void pointers
typedef int (*Comparator)(const void* a, const void* b);

// Implementation:
int CompareInts(const void* a, const void* b) {
    int x = *(const int*)a;
    int y = *(const int*)b;
    return (x > y) - (x < y);
}

// Usage with qsort():
int numbers[] = {3, 1, 4, 1, 5};
qsort(numbers, 5, sizeof(int), CompareInts);
```

### Function Pointer Arrays vs Individual Pointers

#### Individual Function Pointers
```c
typedef void (*StateHandler)(void);

StateHandler idle_handler = Handler_Idle;
StateHandler active_handler = Handler_Active;

// Call individually:
idle_handler();
active_handler();
```

#### Function Pointer Arrays (Jump Tables)
```c
typedef void (*StateHandler)(void);

StateHandler handlers[] = {
    Handler_Idle,    // Index 0
    Handler_Active,  // Index 1
    Handler_Fault    // Index 2
};

// Call via index:
handlers[current_state]();  // Dynamic function call!
```

### Advanced Function Pointer Patterns

#### Nested Function Pointers
```c
// Function that returns a function pointer
typedef int (*Operation)(int, int);
typedef Operation (*OperationSelector)(char op);

Operation GetOperation(char op) {
    switch(op) {
        case '+': return Add;
        case '*': return Multiply;
        default: return NULL;
    }
}

// Usage:
OperationSelector selector = GetOperation;
Operation op = selector('+');
int result = op(5, 3);  // 8
```

#### Function Pointers in Structs
```c
typedef struct {
    char name[20];
    void (*init)(void);
    void (*process)(int data);
    void (*cleanup)(void);
} Module_t;

Module_t wifi_module = {
    .name = "WiFi",
    .init = WiFi_Init,
    .process = WiFi_Process,
    .cleanup = WiFi_Cleanup
};

// Usage:
wifi_module.init();
wifi_module.process(42);
```

### Syntax Comparison Table

| Function Signature | Function Pointer typedef | Array Declaration |
|-------------------|-------------------------|-------------------|
| `void func(void)` | `typedef void (*Handler)(void)` | `Handler table[]` |
| `int func(int, int)` | `typedef int (*Calculator)(int, int)` | `Calculator ops[]` |
| `char* func(const char*)` | `typedef char* (*Processor)(const char*)` | `Processor funcs[]` |
| `void func(int)` | `typedef void (*Callback)(int)` | `Callback events[]` |

### Common Mistakes and Solutions

#### Mistake 1: Forgetting Parentheses
```c
// WRONG:
typedef void *StateHandler(void);  // This is a function returning void*

// CORRECT:
typedef void (*StateHandler)(void); // This is a pointer to function returning void
```

#### Mistake 2: Mismatched Function Signatures
```c
typedef void (*StateHandler)(void);

void BadFunction(int x) { }  // Takes parameter - doesn't match!

StateHandler table[] = {
    Handler_Idle,    // ✓ Matches: void(void)
    BadFunction      // ❌ Error: void(int) doesn't match void(void)
};
```

#### Mistake 3: Not Checking for NULL
```c
StateHandler handler = GetHandler(some_state);
if (handler != NULL) {  // Always check!
    handler();
}
```

### Real-World Automotive Example

```c
// Tesla-style vehicle system management
typedef enum {
    SYS_POWERTRAIN,
    SYS_BATTERY,
    SYS_AUTOPILOT,
    SYS_INFOTAINMENT,
    NUM_SYSTEMS
} SystemType_t;

typedef struct {
    void (*init)(void);
    void (*update)(void);
    void (*sleep)(void);
    void (*emergency_stop)(void);
} SystemInterface_t;

// Jump table for different vehicle systems
const SystemInterface_t SystemTable[NUM_SYSTEMS] = {
    // Powertrain
    { .init = PT_Init, .update = PT_Update, .sleep = PT_Sleep, .emergency_stop = PT_Stop },
    // Battery Management
    { .init = BMS_Init, .update = BMS_Update, .sleep = BMS_Sleep, .emergency_stop = BMS_Stop },
    // Autopilot
    { .init = AP_Init, .update = AP_Update, .sleep = AP_Sleep, .emergency_stop = AP_Stop },
    // Infotainment
    { .init = UI_Init, .update = UI_Update, .sleep = UI_Sleep, .emergency_stop = UI_Stop }
};

// System management becomes trivial:
void UpdateAllSystems(void) {
    for (int i = 0; i < NUM_SYSTEMS; i++) {
        SystemTable[i].update();  // One line updates any system!
    }
}
```

## The Architecture

### 1. **The State**: An enum that serves as an index (0, 1, 2...)
```c
typedef enum {
    STATE_IDLE = 0,
    STATE_ACTIVE,
    STATE_FAULT,
    NUM_STATES  // Helper for array sizing
} State_t;
```

### 2. **The Handler**: Functions that perform logic for specific states
```c
typedef void (*StateHandler)(void);

void Handler_Idle(void);
void Handler_Active(void);
void Handler_Fault(void);
```

### 3. **The Table**: An array of function pointers
```c
const StateHandler State_Table[NUM_STATES] = {
    Handler_Idle,    // Index 0
    Handler_Active,  // Index 1
    Handler_Fault    // Index 2
};
```

### 4. **The Loop**: Direct function call via table lookup
```c
if (current_state < NUM_STATES) {
    State_Table[current_state]();  // Magic happens here!
}
```

## State Behavior Implementation

### IDLE State
- **Purpose**: System waiting/initialization
- **Action**: Print status, increment counter
- **Transition**: When counter > 2, move to ACTIVE
- **Real-world**: Battery monitoring idle state

### ACTIVE State  
- **Purpose**: Main operational mode
- **Action**: Perform primary system tasks
- **Transition**: Immediately move to FAULT (simulating error)
- **Real-world**: Battery charging/discharging operations

### FAULT State
- **Purpose**: Error handling and recovery
- **Action**: Clear errors, reset system
- **Transition**: Return to IDLE for fresh start
- **Real-world**: Overcurrent protection recovery

## Advantages Over Switch-Case

| Aspect | Switch-Case | Function Pointers |
|--------|-------------|-------------------|
| **Readability** | Decreases with states | Stays clean |
| **Maintainability** | Hard to modify | Easy to add/remove |
| **Performance** | O(n) comparison | O(1) direct call |
| **Code Organization** | Monolithic | Modular functions |
| **Testing** | Test entire switch | Test individual handlers |

## Compilation and Execution Instructions

### Step 1: Compile the Program
```bash
cd Day14_CallbackDrivenStateMachine
gcc -o Callback-DrivenStateMachine Cllback-DrivenStateMachine.c
```

### Step 2: Make Executable (if needed)
```bash
chmod +x Callback-DrivenStateMachine
```

### Step 3: Run the Program
```bash
./Callback-DrivenStateMachine
```

### Expected Output
```
--- Final Challenge: Function Pointer FSM ---
[IDLE] Waiting... (Count: 0)
[IDLE] Waiting... (Count: 1)
[IDLE] Waiting... (Count: 2)
[ACTIVE] Doing heavy work...
[FAULT] Clearing errors...
[IDLE] Waiting... (Count: 0)
[IDLE] Waiting... (Count: 1)
[IDLE] Waiting... (Count: 2)
[ACTIVE] Doing heavy work...
[FAULT] Clearing errors...
```

## Real-World Applications

### Automotive Systems
- **Engine Control Units (ECU)**: Idle → Running → Fault states
- **Battery Management Systems (BMS)**: Standby → Charging → Balancing → Protection
- **Anti-lock Braking Systems (ABS)**: Normal → Monitoring → Active → Recovery

### Tesla Example Pattern
```c
// Tesla-style state machine architecture
typedef enum {
    VEHICLE_PARKED,
    VEHICLE_DRIVING,
    VEHICLE_CHARGING,
    VEHICLE_AUTOPILOT,
    VEHICLE_EMERGENCY,
    NUM_VEHICLE_STATES
} VehicleState_t;

const StateHandler VehicleStateTable[NUM_VEHICLE_STATES] = {
    Handle_Parked,
    Handle_Driving,
    Handle_Charging,
    Handle_Autopilot,
    Handle_Emergency
};
```

## Key Learning Points

1. **Scalability**: Easy to add new states without modifying existing code
2. **Performance**: Direct function calls are faster than switch statements
3. **Maintainability**: Each state handler is independent and testable
4. **Safety**: Enum indexing prevents invalid state access
5. **Industry Standard**: Used in mission-critical automotive systems

## Extension Ideas

1. **State Transition Matrix**: Add validation for legal state transitions
2. **State Entry/Exit Actions**: Add callbacks for state changes
3. **Hierarchical States**: Implement sub-states within main states
4. **Event-Driven**: Add event parameters to state handlers
5. **State History**: Track state transition history for debugging

## Safety Considerations

- Always validate state index before table access
- Use `const` for state table to prevent modification
- Consider state transition validation in safety-critical systems
- Implement watchdog timers for stuck state detection

This architecture demonstrates professional-grade embedded system design used in automotive applications where reliability and maintainability are paramount.