#ifndef __NONEPLANE_H__
#define __NONEPLANE_H__
#include <iostream>
#include "airplane.h"
namespace avdance {
    class NonePlane : public Airplane {
        public:
            NonePlane () {
                std::cout << "NonePlane构造" << std::endl;
                this->weapons = 1;
            }
            ~NonePlane () {
                std::cout << "NonePlane析构" << std::endl;
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