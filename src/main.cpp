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

/* ALARM */
class Alarm {
public:
    bool low;
    bool medium;
    bool high;

    int pattern_loc = 0;
    bool restart_pattern;

    std::string low_pattern;
    std::string medium_pattern;
    std::string high_pattern;

    // print output
    void output() {
        while(true) {
            if (restart_pattern) {
                // when an alarm is triggered, start at the pattern beginning
                pattern_loc = 0;
                restart_pattern = false;
            }
            if (high) {
                std::cout << high_pattern[pattern_loc % high_pattern.length()] << std::flush;
            } else if (medium) {
                std::cout << medium_pattern[pattern_loc % medium_pattern.length()] << std::flush;
            } else if (low) {
                std::cout << low_pattern[pattern_loc % low_pattern.length()] << std::flush;
            } else {
                std::cout << "_" << std::flush;
            }

            // increment timer and pointer and sleep
            easy_time ++;
            pattern_loc ++;
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    }

    // listen for input
    void input() {
        char user_input;
        while(true) {
            user_input = getchar();
            // toggle states
            if (user_input == 'h') {
                high = !high;
                restart_pattern = true;
            } else if (user_input == 'm') {
                medium = !medium;
                restart_pattern = true;
            } else if (user_input == 'l') {
                low = !low;
                restart_pattern = true;
            };
        }
    }

    std::thread output_t() {
        return std::thread([=] {output();});
    }

    std::thread input_t() {
        return std::thread([=] {input();});
    }

    void make_patterns() {
        low_pattern = "XXXX";
            for (int i=0; i < 29; i++) {low_pattern.append("____");};
        medium_pattern = "X___";
        high_pattern = "X_X_X_X_X_________";
    }

};

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
