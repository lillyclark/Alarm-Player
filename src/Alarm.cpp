#include "Alarm.h"
#include <iostream>

// constructor makes the pattern strings
Alarm::Alarm() {
    off_pattern = "_";
    low_pattern = "XXXX";
        for (int i=0; i < 29; i++) {low_pattern.append("____");};
    medium_pattern = "X___";
    high_pattern = "X_X_X_X_X_________";
};

// alarm threads
void Alarm::start() {
    output_thread = std::thread([=] {output();});
    input_thread = std::thread([=] {input();});
    output_thread.join();
    input_thread.join();
}

// output summary
void Alarm::stop() {
    std::cout << std::endl << "Elapsed time: " << easy_time*250 << " ms ";
    std::cout << "(" << easy_time << " characters printed)" << std::endl;
}

// print output
void Alarm::output() {
    while(true) {
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
}

// listen for input
void Alarm::input() {
    char user_input;
    while(true) {
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
