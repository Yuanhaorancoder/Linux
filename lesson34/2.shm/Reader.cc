// header only

#include "Shm.hpp"

#include <iostream>
#include <string>
#include <unistd.h>

// Writer -> shm -> Reader
int main()
{
    // 1.在内核中创建共享内存
    Shm shm;
    shm.Create();

    sleep(3);
    
    shm.Attach();
    
    // shm.Debug();
    // shm.GetShmAttr();

    sleep(5);

    shm.Delete();

    return 0;
}