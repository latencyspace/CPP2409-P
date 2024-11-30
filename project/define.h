#ifndef DEFINE_LIBRARY
#define DEFINE_LIBRARY

#include <iostream>
#include <stdlib.h>

using namespace std;

#define clearScreen system("cls");
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x));
#define horizontalFill cout << "****************************************************\n";
#define horizontalBlank cout << "*                                                  *\n";

#endif
