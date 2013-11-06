#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    sleep(1000);
    
    char buf[255];
    sprintf(buf, "time is: %lu\n", time());
    
    write(STDOUT_FILENO, buf, strlen(buf));
    
    return 0;
}
