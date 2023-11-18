#include <iostream>
#include "nonePlane.h"

namespace avdance {
    int NonePlane::getWeapons() {
        return this->weapons;
    }

    void NonePlane::setWeapons(int w) {
        this->weapons = w;
    }

    void NonePlane::fly() {
        std::cout << "NonePlane flying..." << std::endl;
    }
}