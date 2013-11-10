#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    printf("Curr time is: %lu.\nNow sleeping for 2000ms...\n", time());
    sleep(2000);
    printf("Curr time is: %lu.\n Now sleeping for 15 minutes...\n", time());
    sleep(900000);
    printf("Done! Curr time is: %lu\n", time());
    
    return 0;
}
