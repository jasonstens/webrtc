#include <iostream>
#include "fightPlane.h"

using namespace avdance;

int FightPlane::getWeapons() {
    return this->weapons;
}

void FightPlane::setWeapons(int w) {
    this->weapons = w;
}

void FightPlane::fly() {
    std::cout << "fightplane flying..." << std::endl;
}