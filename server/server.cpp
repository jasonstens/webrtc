#include <iostream>
#include <unistd.h>
#include "server.h"

namespace avdance {
Server::Server() {
    std::cout << "construct..." << std::endl;
}

Server::~Server() {
    std::cout << "destruct..." << std::endl;
}

void Server::run() {
    while (true) {
        std::cout << "runing..." << std::endl;
        ::usleep(1000000);
    }
}
}