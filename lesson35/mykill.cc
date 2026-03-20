#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>

void Usage(std::string proc)
{
    std::cerr << "Usage:\n\t" << proc << " signumber pid\n\n";
}

// ./mykill signumber pid
// ./mykill  9  1234567
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }
    int signumber = std::stoi(argv[1]);
    pid_t pid = std::stoi(argv[2]);

    std::cout << "send " << signumber << " to " << pid << std::endl;
    int n=kill(pid,signumber);
    (void)n;

    return 0;
}