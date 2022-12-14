#ifndef ALARM_H
#define ALARM_H
#include <thread>
#include <stdio.h>

/* ALARM */
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

    // constructor
    Alarm();

    // destructor
    ~Alarm();

    // print output
    void output();

    // listen for input
    void input();

    // make output thread
    std::thread output_t();

    // make input thread
    std::thread input_t();

    // set up alarm patterns
    void make_patterns();

};
#endif
