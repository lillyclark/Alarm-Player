#ifndef ALARM_H
#define ALARM_H
#include <thread>
#include <stdio.h>

class Alarm {
public:
    bool low;
    bool medium;
    bool high;

    bool running;

    int easy_time;
    int pattern_loc;
    bool restart_pattern;

    std::string off_pattern;
    std::string low_pattern;
    std::string medium_pattern;
    std::string high_pattern;

    std::thread output_thread;
    std::thread input_thread;

    // constructor
    Alarm();

    // start
    void start();

    // for testing
    void start_timed(int secs);

    // stop
    void stop();

    // print output
    // if max_time is zero, runs forever
    void output(int max_time);

    // listen for input
    void input();

    // set priority
    void set_priority(char user_input);

    // get priority (0-off, 1-low, 2-med, 3-high)
    int get_priority();

    // get pattern
    std::string get_pattern(int priority);
};
#endif
