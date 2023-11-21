#include <iostream>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (daemon(0, 0) < 0)
    {
        std::cout << "daemon create error!" << std::endl;
    }

    while (true)
    {
        sleep(1);
    }

    return 0;
}