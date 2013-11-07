#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    sleep(1000);
    printf("time is: %lu\n", time());
    
    sleep(2000);
    printf("new time is: %lu\n", time());
    
    printf("Whee end of time!\n");
    
    return 0;
}
