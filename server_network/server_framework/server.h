#ifndef __SERVER_H__
#define __SERVER_H__

#include <iostream>

namespace avdance {

class Server {
    public:
        Server();
        ~Server();
    
    public:
        void run();
    
    private:
        int id;
};
}
#endif