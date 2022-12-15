/* really simple unit testing :) */

#include "Alarm.h"
#include <stdio.h>
#include <iostream>
#include <unistd.h>

int TestIsOff(Alarm *a) {
    return (a->get_priority() == 0) && (a->get_pattern(0) == "_");
}

int TestIsLow(Alarm *a) {
    return (a->get_priority() == 1) &&
        (a->get_pattern(1) == "XXXX____________________________________________________________________________________________________________________");
}

int TestIsMed(Alarm *a) {
    return (a->get_priority() == 2) &&
        (a->get_pattern(2) == "X___");
}

int TestIsHigh(Alarm *a) {
    return (a->get_priority() == 3) &&
        (a->get_pattern(3) == "X_X_X_X_X_________");
}

int TestTimeElapsed(Alarm *a, int ms) {
    int elapsed = a->check_time();
    int epsilon = 20; // mercy in ms
    std::cout << "Timer drifted by " << (elapsed*250 - ms) << " ms." << std::endl;
    return (elapsed*250 - ms < epsilon);
}

int main(){

    // create alarm
    Alarm *a = new Alarm();
    int count_successes = 0;
    int count_tests = 0;

    /* TEST INITIALIZATION */

    if (TestIsOff(a)) {
        count_successes++;
    } else {std::cerr << "Alarm did not start in 'off'." << std::endl;}
    count_tests ++;

    /* TEST STARTING EACH ALARM */

    // simulate 'l'
    a->set_priority('l');
    if (TestIsLow(a)) {
        count_successes++;
    } else {std::cerr << "'l' did not set low alarm." << std::endl;}
    count_tests ++;

    // simulate 'm'
    a->set_priority('m');
    if (TestIsMed(a)) {
        count_successes++;
    } else {std::cerr << "'m' did not set med alarm." << std::endl;}
    count_tests ++;

    // simulate 'h'
    a->set_priority('h');
    if (TestIsHigh(a)) {
        count_successes++;
    } else {std::cerr << "'h' did not set high alarm." << std::endl;}
    count_tests ++;

    /* TEST STOPPING EACH ALARM */

    // simulate 'h'
    a->set_priority('h');
    if (TestIsMed(a)) {
        count_successes++;
    } else {std::cerr << "'h' did not toggle high alarm." << std::endl;}
    count_tests ++;

    // simulate 'm'
    a->set_priority('m');
    if (TestIsLow(a)) {
        count_successes++;
    } else {std::cerr << "'m' did not toggle med alarm." << std::endl;}
    count_tests ++;

    // simulate 'l'
    a->set_priority('l');
    if (TestIsOff(a)) {
        count_successes++;
    } else {std::cerr << "'l' did not toggle low alarm." << std::endl;}
    count_tests ++;

    /* TEST PRIORITIES */

    a->set_priority('h');
    a->set_priority('m');
    a->set_priority('l');
    a->set_priority('m');
    a->set_priority('l');
    if (TestIsHigh(a)) {
        count_successes++;
    } else {std::cerr << "'h' did not maintain priority." << std::endl;}
    count_tests ++;
    a->set_priority('h'); // toggle off

    std::cout << count_successes << " out of " <<  count_tests << " tests passed!" << std::endl;

    /* TEST TIMING */
    std::cout << "starting 10 second alarm (no user input accepted)..." << std::endl;

    // start stopwatch
    auto start = std::chrono::high_resolution_clock::now();

    // run for 40 units (each 250ms)
    a->start_timed(40);

    // stop startwatch
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);

    if (TestTimeElapsed(a, duration.count())) {
        count_successes++;
    } else {std::cerr << "timer drifted too much :(" << std::endl;}
    count_tests ++;

    // Summary of all tests
    std::cout << count_successes << " out of " <<  count_tests << " tests passed!" << std::endl;

    return 0;

}
