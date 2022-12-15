#include "Alarm.h"

#include <iostream>
#include <unistd.h>
#include <signal.h>

// create alarm
Alarm *myAlarm = new Alarm();

// stop the alarm on SIGINT
void catch_interrupt(int signum) {
    myAlarm->stop();
    exit(signum);
}

int main()
{
    // specify what to do on ctrl-c
    signal(SIGINT, catch_interrupt);

    // start alarm
    myAlarm->start();

    // program will only exit on ctrl-c
    return 0;
}
