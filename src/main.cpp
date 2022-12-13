#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void catch_interrupt(int signum) {
    std::cout << std::endl << "Elapsed time:" << std::endl;
    exit(signum);
}

class Alarm {
public:
    bool low;
    bool medium;
    bool high;

    void output() {
        while(true) {
            if (medium) {
                std::cout << "X" << std::flush;
            } else {
                std::cout << "_" << std::flush;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    }

    void input() {
        char user_input;
        while(true) {
            std::cin >> user_input;
            std::cout << "*" << std::flush;
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
    signal(SIGINT, catch_interrupt);

    Alarm *alarm = new Alarm();
    std::thread output_thread = alarm->output_t();
    std::thread input_thread = alarm->input_t();

    output_thread.join();
    input_thread.join();
    std::cout<<"Done"<<std::endl;
    return 0;
}
