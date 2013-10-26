/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    10 Oct 2013
 */

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

// Fix a buffer size.
#define BUF_SIZE 64

// Applies ROT13 encoding to the input char array in-place.
int applyROT13(char *input, unsigned int len) {
    int i;

    for (i = 0; i < len; i++) {
        if ((65 <= input[i] && input[i] <= 77) ||
            (97 <= input[i] && input[i] <= 109)) {
            input[i] += 13;
        } else if ((78 <= input[i] && input[i] <= 90) ||
                   (110 <= input[i] && input[i] <= 122)) {
            input[i] -= 13;
        }
    }

    return 0;
}

void write_string(char* buffer, unsigned int str_len) {
    // Loop and write until we have finished writing everything in
    // the buffer.
    int writeSize;
    if (str_len != 0) {
        do {
            if ((writeSize = write(STDOUT_FILENO, buffer, str_len)) < 0) {
                exit(1);
            }
            str_len -= writeSize;
        } while (str_len > 0);
    }
}

int main(int argc, char** argv) {
    char buf[BUF_SIZE];
    int readSize, i;
    unsigned int str_len;
    char* plain_text = "plain text: ";
    char* cipher_text = "cipher text: ";
    char* new_line = "\n";
    i = 0;

    // Do initial encryption of command line arguments.
    for (i = 1; i < argc; i++) {
        // Write "plain text: "
        write_string(plain_text, 12);
        str_len = 0;
        // Calculate the string length.
        while (argv[i][str_len] != 0) {
            str_len++;
        }

        // Write the plain text string.
        write_string(argv[i], str_len);
        write_string(new_line, 1);

        write_string(cipher_text, 13);
        applyROT13(argv[i], str_len);
        // Write to stdout what we have so far.
        write_string(argv[i], str_len);
        write_string(new_line, 1);
        // Insert a new line to separate the words.
        write_string(new_line, 1);
    }

    while (1) {
        // Reset input size.
        readSize = 0;
        // Read input from stdin until we reach EOF (readSize = 0)
        do {
            write_string(plain_text, 12);
            if ((readSize = read(STDIN_FILENO, buf, BUF_SIZE)) < 0) {
                exit(1);
            }
            write_string(cipher_text, 13);
            // Do ROT13
            applyROT13(buf, readSize);
            // Write to stdout what we have so far.
            if (readSize != 0) {
                write_string(buf, readSize);
            }
            write_string(new_line, 1);
        } while (readSize != 0);
    }
}
