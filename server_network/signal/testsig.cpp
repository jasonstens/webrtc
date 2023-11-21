#include <iostream>
#include <unistd.h>
#include <signal.h>

void handler(int sig) {
    std::cout << "get sig, sig is " << sig << std::endl;
}

int main(int argc, char* argv[]) {

    signal(SIGINT, handler);
    signal(SIGHUP, handler);
    signal(SIGQUIT, handler);

    pause();

    return 0;
}