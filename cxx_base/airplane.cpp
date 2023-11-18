#include <iostream>
#include "airplane.h"

namespace avdance {
    void Airplane::setWings(int w) {
        this->wings = w;
    }

    int Airplane::getWings() {
        return this->wings;
    }

    void Airplane::setEngines(int e) {
        this->engines = e;
    }

    int Airplane::getEngines() {
        return this->engines;
    }
    
    void Airplane::fly() {
        std::cout << "airplane flying..." << std::endl;
    }
};