#include "Alarm.h"

#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <signal.h>

// to store terminal input settings
struct termios oldt, newt;

// create alarm
Alarm *myAlarm = new Alarm();

void catch_interrupt(int signum) {
    // reset the terminal input settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    myAlarm->stop();
    exit(signum);
}

int main()
{
    // save the terminal input settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // don't echo input, and allow chars one at a time
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // specify what to do on ctrl-c
    signal(SIGINT, catch_interrupt);

    // start alarm
    myAlarm->start();

    // program will only exit on ctrl-c
    return 0;
}
