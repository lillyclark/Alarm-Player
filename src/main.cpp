#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

struct termios oldt, newt;

// exit gracefully
void catch_interrupt(int signum) {
    // reset the terminal input settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    // print out summary
    std::cout << std::endl << "Elapsed time:" << std::endl;
    exit(signum);
}

/* ALARM */
class Alarm {
public:
    bool low;
    bool medium;
    bool high;
    int easy_time = 0;

    // print output
    void output() {
        while(true) {
            easy_time ++;

            if (high) {
                std::cout << "X" << std::flush;
            } else if (medium) {
                std::cout << "x" << std::flush;
            } else if (low) {
                std::cout << "." << std::flush;
            } else {
                std::cout << "_" << std::flush;
            }
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
            } else if (user_input == 'm') {
                medium = !medium;
            } else if (user_input == 'l') {
                low = !low;
            };
        }
    }

    std::thread output_t() {
        return std::thread([=] {output();});
    }

    std::thread input_t() {
        return std::thread([=] {input();});
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

    Alarm *alarm = new Alarm();
    std::thread output_thread = alarm->output_t();
    std::thread input_thread = alarm->input_t();

    output_thread.join();
    input_thread.join();
    std::cout<<"Done"<<std::endl;
    return 0;
}
