#ifndef __FIGHTPLANE_H__
#define __FIGHTPLANE_H__
#include <iostream>
#include "airplane.h"
namespace avdance {
    class FightPlane : public Airplane {
        public:
            FightPlane () {
                std::cout << "FightPlane构造" << std::endl;
                this->weapons = 1;
            }
            ~FightPlane () {
                std::cout << "FightPlane析构" << std::endl;
            }
        public:
            void setWeapons(int w);
            int getWeapons();
        public:
            virtual void fly();
        private:
            int weapons;
    };
}


#endif