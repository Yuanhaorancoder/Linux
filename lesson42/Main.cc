#include <iostream>
#include <unistd.h>
#include <vector>
#include "Thread.hpp"

void hello()
{
    while(true)
    {
        std::cout<<"hello thread"<<std::endl;
        sleep(1);
    }
}

int main()
{
    std::vector<Thread> threads; 
    for(int i=0;i<10;i++)
    {
        threads.emplace_back(hello);
    }

    for(auto &thread:threads)
        thread.start();

    for(auto &thread:threads)
        thread.join();

    // Thread t(hello);
    // t.start();
    
    // sleep(5);

    // t.stop();
    
    // t.join();
    // sleep(3);

    return 0;
}
