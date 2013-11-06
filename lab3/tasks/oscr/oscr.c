#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    char buf[255];
    while(1) {
        sprintf(buf, "%lu\n", time());
        write(STDOUT_FILENO, buf, strlen(buf));
    };
    return 0;
}
