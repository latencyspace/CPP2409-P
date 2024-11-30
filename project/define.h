#ifndef DEFINE_LIBRARY
#define DEFINE_LIBRARY

#include <iostream>
#include <stdlib.h>

using namespace std;

#define clear system("cls");                          // 화면 초기화
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x)); // 커서 위치 지정
#define horizontalFill cout << "****************************************************\n";
#define horizontalBlank cout << "*                                                  *\n";

#endif
