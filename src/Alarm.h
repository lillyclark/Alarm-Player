#ifndef ALARM_H
#define ALARM_H
#include <thread>
#include <string>
#include <stdio.h>
#include <termios.h>

class Alarm {
private:
    // priority indicators
    bool low;
    bool medium;
    bool high;

    // private flags and counters
    bool running;
    int easy_time;
    int pattern_loc;
    bool restart_pattern;

    // alarm patterns
    std::string off_pattern;
    std::string low_pattern;
    std::string medium_pattern;
    std::string high_pattern;

    // threads for input and output
    std::thread output_thread;
    std::thread input_thread;

    // terminal settings
    struct termios oldt, newt;

    // print output, if max_time is zero, runs forever
    void output(int max_time);

    // listen for input
    void input();

public:
    // constructor
    Alarm();

    // start
    void start();

    // for testing
    void start_timed(int secs);

    // stop
    void stop();

    // set priority (takes any character as input)
    void set_priority(char user_input);

    // get priority (0-off, 1-low, 2-med, 3-high)
    int get_priority();

    // get pattern (0-off, 1-low, 2-med, 3-high)
    std::string get_pattern(int priority);

    // check time elapsed (in iterations)
    int check_time();
};
#endif
