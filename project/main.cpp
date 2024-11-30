// /usr/bin/g++ -std=c++14 -fdiagnostics-color=always -g /Users/latency/Desktop/Dev/C++/CPP2409-P/project/main.cpp -o /Users/latency/Desktop/Dev/C++/CPP2409-P/project/main

#include "define.h"
#include "files.h"

class run
{
private:
    char input;

    void border()
    {
        clear
                horizontalFill
                    horizontalBlank
                        horizontalFill
                            cout
            << "Welcome to the Music Player!" << endl;
        cout << "Press SPACE to exit or any key to continue..." << endl;
        input = getchar();
        if (input == ' ')
        {
            clear;
            exit(0);
        }
    }

public:
    run() { border(); }
};

int main(void)
{
    run MusicPlayer;
    return 0;
}
