#define _POSIX_C_SOURCE 200809L

#include <unistd.h>

#include "watchdog.h"
#include "utility.h"


int main(int argc, char *argv[])
{
    watchdog_t *watchdog;

    UNUSED(argc);
    watchdog = WatchdogStart(argv[0]);

    sleep(60);

    WatchdogStop(watchdog);





    return 0;
}