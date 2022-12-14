#ifndef ALARM_H
#define ALARM_H
#include <thread>
#include <stdio.h>

class Alarm {
public:
    bool low;
    bool medium;
    bool high;

    int easy_time;
    int pattern_loc;
    bool restart_pattern;

    std::string low_pattern;
    std::string medium_pattern;
    std::string high_pattern;

    std::thread output_thread;
    std::thread input_thread;

    // constructor
    Alarm();

    // start
    void start();

    // print output
    void output();

    // listen for input
    void input();
};
#endif
