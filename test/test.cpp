// /usr/bin/g++ -std=c++14 -fdiagnostics-color=always -g /Users/latency/Desktop/Dev/C++/CPP2409-P/test/test.cpp -o /Users/latency/Desktop/Dev/C++/CPP2409-P/test/test

#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <cstdlib> // system() 사용을 위한 헤더 추가

using namespace std;

volatile sig_atomic_t resize_flag = 0;

void handleResize(int sig)
{
    resize_flag = 1; // 신호가 발생하면 플래그를 설정
}

void displayMenu(const string &currentSong)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int width = w.ws_col;

    string line(width, '=');

    cout << "\033[H\033[J"; // 화면 지우기
    cout << line << "\n";
    cout << "Now playing: " << currentSong << "\n";
    cout << line << "\n";
    cout << "1. 이전 곡\n";
    cout << "2. 재생/일시정지\n";
    cout << "3. 다음 곡\n";
    cout << "4. 플레이리스트\n";
    cout << "0. 프로그램 종료\n";
    cout << line << "\n";
    cout << "무엇을 실행하시겠어요?: ";
}

void setupEnvironment()
{
    system("python3 -m venv path/to/venv");
    system("source path/to/venv/bin/activate");
    system("path/to/venv/bin/pip install yt-dlp");
}

int main()
{
    setupEnvironment(); // 환경 설정 함수 호출

    string currentSong = "예시 음악 제목";
    string userInput;

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handleResize;
    sigaction(SIGWINCH, &sa, nullptr);

    displayMenu(currentSong);

    while (true)
    {
        if (resize_flag)
        {
            resize_flag = 0;
            displayMenu(currentSong);
        }

        getline(cin, userInput);

        if (userInput == "1")
        {
            cout << "이전 곡 기능 실행\n";
        }
        else if (userInput == "2")
        {
            cout << "재생/일시정지 기능 실행\n";
        }
        else if (userInput == "3")
        {
            cout << "다음 곡 기능 실행\n";
        }
        else if (userInput == "4")
        {
            cout << "플레이리스트 기능 실행\n";
        }
        else if (userInput == "0")
        {
            cout << "프로그램 종료\n";
            break;
        }
        else
        {
            cout << "잘못된 입력입니다. 다시 시도하세요.\n";
        }

        displayMenu(currentSong);
    }

    return 0;
}