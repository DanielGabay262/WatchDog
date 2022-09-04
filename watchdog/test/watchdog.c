#define _POSIX_C_SOURCE 200809L

#include <stdio.h> /*sprintf*/
#include <stdlib.h> /*setenv*/
#include <unistd.h> /*getpid*/

#include "watchdog.h"
#include "utility.h"

#define MAX 50


int main(int argc, char *argv[])
{
    char buffer[MAX] = {0};

    UNUSED(argc);
    sprintf(buffer, "STATUS - RUN, PID - %d", getpid());
    setenv("WD_PID", buffer, 0);
    #ifndef NDEBUG
    printf("dog process just opened!!\n");
    printf("dog pid: %d\n", getpid());
    #endif

    WatchdogStart(argv[1]);
    return 0;
}