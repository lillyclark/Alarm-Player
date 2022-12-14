#include "Alarm.h"

#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

// easiest way to exit gracefully is to use global vars
struct termios oldt, newt;
int easy_time = 0;

void catch_interrupt(int signum) {
    // reset the terminal input settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    // print out summary
    std::cout << std::endl << "Elapsed time: " << easy_time*250 << " ms ";
    std::cout << "(" << easy_time << " characters printed)" << std::endl;
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

    // create alarm and start input and output threads
    Alarm *alarm = new Alarm();
    alarm->make_patterns();
    std::thread output_thread = alarm->output_t();
    std::thread input_thread = alarm->input_t();

    // program will only exit on ctrl-c
    output_thread.join();
    input_thread.join();
    return 0;
}
