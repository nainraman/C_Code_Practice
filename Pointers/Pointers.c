#include <stdio.h>
#include <stdlib.h> // Required for malloc() and free()

int main() {
    // Declare a pointer to an integer
    int *ptr;

    // Allocate memory for a single integer
    // sizeof(int) determines the size in bytes required for an int (usually 4 bytes)
    // The result of malloc is cast to an int pointer (optional in modern C, but common)
    ptr = (int*) malloc(sizeof(int));

    // Check if the memory allocation was successful
    if (ptr == NULL) {
        printf("Error! Memory allocation failed.\\n");
        return 1; // Exit the program with an error code
    }

    // Assign a value to the allocated memory location
    *ptr = 42;

    // Print the value stored in the allocated memory
    printf("Value stored in allocated memory: %d\\n", *ptr);

    // Deallocate the memory using free()
    free(ptr);
    
    // Optional: set the pointer to NULL after freeing to prevent accidental use (dangling pointer)
    ptr = NULL;

    return 0;
}
