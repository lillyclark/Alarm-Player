#include "Alarm.h"

#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

// constructor
Alarm::Alarm() {
    std::cout << "Create alarm" << std::endl;
};

// destructor
Alarm::~Alarm() {
    std::cout << "Destroy alarm" << std::endl;
};

// print output
void Alarm::output() {
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
void Alarm::input() {
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

std::thread Alarm::output_t() {
    return std::thread([=] {output();});
}

std::thread Alarm::input_t() {
    return std::thread([=] {input();});
}

void Alarm::make_patterns() {
    low_pattern = "XXXX";
        for (int i=0; i < 29; i++) {low_pattern.append("____");};
    medium_pattern = "X___";
    high_pattern = "X_X_X_X_X_________";
}
