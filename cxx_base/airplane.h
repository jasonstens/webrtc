#ifndef __AIRPLANE_H__
#define __AIRPLANE_H__
#include <iostream>

namespace avdance
{
    class Airplane {
    public:
        Airplane(){
            this->wings = 2;
            this->engines = 1;
            this->wheels = 3;
            std::cout << "Airplane构造" << std::endl;
       
        };
        ~Airplane(){
            std::cout << "Airplane析构" << std::endl;
        };

    public:
        void setWings(int w);
        void setEngines(int e);
        int getWings();
        int getEngines();

    public:
        virtual void fly();

    private:
        int wings;
        int engines;
        int wheels;
    };

}

#endif