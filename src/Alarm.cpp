#include "Alarm.h"
#include <iostream>
#include <unistd.h>

// constructor makes the pattern strings, stores terminal settings
Alarm::Alarm() {
    off_pattern = "_";
    low_pattern = "XXXX";
        for (int i=0; i < 29; i++) {low_pattern.append("____");};
    medium_pattern = "X___";
    high_pattern = "X_X_X_X_X_________";

    // save the terminal input settings
    tcgetattr(STDIN_FILENO, &oldt);
};

// start the alarm
void Alarm::start() {
    // don't echo input, and allow chars one at a time
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // start threads
    running = true;
    output_thread = std::thread([=] {output(0);});
    input_thread = std::thread([=] {input();});
    output_thread.join();
    input_thread.join();
}

// start only the output thread for debugging (don't change terminal settings)
void Alarm::start_timed(int secs) {
    running = true;
    output_thread = std::thread([=] {output(secs);});
    output_thread.join();
    stop();
}

// stop the alarm
void Alarm::stop() {
    // reset the terminal input settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    // print summary
    std::cout << std::endl << "Elapsed time: " << easy_time*250 << " ms ";
    std::cout << "(" << easy_time << " characters printed)" << std::endl;
}

// print output
// max time of 0 lets this run forever
void Alarm::output(int max_time) {
    while(running && (max_time == 0 || easy_time < max_time)) {
        if (restart_pattern) {
            // when an alarm is triggered, start at the pattern beginning
            pattern_loc = 0;
            restart_pattern = false;
        }

        // print current pattern at current pointer
        int priority = get_priority();
        std::string pattern = get_pattern(priority);
        std::cout << pattern[pattern_loc % pattern.length()] << std::flush;

        // increment timer and pointer and sleep
        easy_time ++;
        pattern_loc ++;
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
    running = false;
}

// listen for input
void Alarm::input() {
    char user_input;
    while(running) {
        user_input = getchar();
        set_priority(user_input);
    }
}

// set priority from char
void Alarm::set_priority(char user_input) {
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

// get current priority
int Alarm::get_priority() {
    if (high) {
        return 3;
    } else if (medium) {
        return 2;
    } else if (low) {
        return 1;
    } return 0;
}

// returns the string pattern from priority level
std::string Alarm::get_pattern(int priority) {
    if (high) {
        return high_pattern;
    } else if (medium) {
        return medium_pattern;
    } else if (low) {
        return low_pattern;
    } else {
        return off_pattern;
    }
}

int Alarm::check_time() {
    return easy_time;
}
