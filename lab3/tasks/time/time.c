#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    printf("start timing!\n");
    
    sleep(10000);
    printf("new time is: %lu\n", time());
    
    return 0;
}
