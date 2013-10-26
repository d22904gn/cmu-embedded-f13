#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

// Writes a string to stdout.
void write_string(char *str, unsigned int len) {
    int write_size = 0;
    
    do {
        if ((write_size = write(STDOUT_FILENO, str, len)) < 0) {
            exit(1);
        }
        len -= write_size;
    } while (len > 0);
}

int main(int argc, char **argv) {
    int ret = 0;
    char buf[] = "a";
    
    ret = write(-1, buf, 1);
    exit(ret);
    
    return 123;
}
