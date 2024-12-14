#ifndef DEFINE_LIBRARY
#define DEFINE_LIBRARY

#include <iostream>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

#ifdef _WIN32
#define clearScreen system("cls")
inline void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
#else
#define clearScreen system("clear")
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))
#endif

#define horizontalFill cout << "****************************************************\n";
#define horizontalBlank cout << "*                                                  *\n";

#endif
