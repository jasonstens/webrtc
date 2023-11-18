#include<iostream>
#include "airplane.h"
#include "fightPlane.h"
#include "nonePlane.h"

int main(int argc, char* argv[]) {
    // avdance::FightPlane fp;

    // std::cout << "engines:" << fp.getEngines() << std::endl;
    // std::cout << "weapons:" << fp.getWeapons() << std::endl;
    // std::cout << "wings:" << fp.getWings() << std::endl;

    // fp.setWeapons(10);
    // fp.setEngines(5);
    // std::cout << "new weapons:" << fp.getWeapons() << std::endl;
    // std::cout << "new engines:" << fp.getEngines() << std::endl;

    avdance::Airplane *ap = new avdance::FightPlane();
    if (ap != nullptr) {
        ap->fly();
    }

    ap = new avdance::NonePlane();
    if (ap != nullptr) {
        ap->fly();
    }

    ap = new avdance::Airplane();
    if (ap != nullptr) {
        ap->fly();
    }
    return 0;
}