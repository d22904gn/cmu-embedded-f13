#include <stdio.h>
#include "math.h"

int main(int argc, char *argv[]) {
    int a = 0, b = 0, result = 0;
    char op = '\0';
    char buf[255];
    
    // Keep reading until an invalid input is detected.
    while (1) {
        // Read user input to a buffer.
        fgets(buf, 255, stdin);
        
        // Parse user input, making sure to check that match is successful.
        if (sscanf(buf, "%d %c %d", &a, &op, &b) != 3) {
            // Invalid input detected. Exit.
            return 0;
        }
        
        // Check which operation user requested and print result.
        switch (op) {
        case '+':
            result = add(a, b);
            break;
        case '-':
            result = sub(a, b);
            break;
        case '*':
            result = mul(a, b);
            break;
        case '/':
            result = cdiv(a, b);
            break;
        case '%':
            result = mod(a, b);
            break;
        default:
            // Invalid operator detected. Exit.
            return 0;
        }
        
        printf("%d\n\n", result);
    }
}

