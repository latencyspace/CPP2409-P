#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>

using namespace std;

volatile sig_atomic_t resize_flag = 0;

void handleResize(int sig)
{
    resize_flag = 1; // 신호가 발생하면 플래그를 설정
}

void displayMenu(const string &currentSong)
{
    // 콘솔 창의 너비 가져오기
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int width = w.ws_col; // 현재 창의 너비

    // 동적으로 = 문자 생성
    string line(width, '=');

    cout << "\033[H\033[J"; // 화면 지우기
    cout << line << "\n";
    cout << "Now playing: " << currentSong << "\n";
    cout << "이전 곡 재생하기 (prev)\n";
    cout << "음악 재생/일시정지 (p)\n";
    cout << "다음 곡 재생하기 (next)\n";
    cout << line << "\n";
    cout << "1. 플레이리스트 확인하기\n";
    cout << "0. 프로그램 종료\n";
    cout << line << "\n";
    cout << "실행하고자 하는 기능에 대한 문자를 입력하세요: "; // 사용자 입력을 받는 라인
}

int main()
{
    string currentSong = "예시 음악 제목"; // 현재 음악 제목을 여기에 설정
    string userInput;                      // 사용자 입력 변수

    // SIGWINCH 신호를 처리하기 위한 핸들러 등록
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handleResize;
    sigaction(SIGWINCH, &sa, nullptr);

    // 프로그램 시작 시 화면 출력
    displayMenu(currentSong);

    while (true)
    {
        if (resize_flag)
        {
            resize_flag = 0;          // 플래그 초기화
            displayMenu(currentSong); // 화면 갱신
        }

        // 사용자 입력 처리
        getline(cin, userInput); // 사용자 입력 받기

        // 입력에 따라 행동 결정
        if (userInput == "0")
        {
            cout << "프로그램을 종료합니다.\n";
            break; // 프로그램 종료
        }
        else if (userInput == "1")
        {
            cout << "플레이리스트를 확인하는 기능은 아직 구현되지 않았습니다.\n";
        }
        else if (userInput == "prev")
        {
            cout << "이전 곡 재생 기능은 아직 구현되지 않았습니다.\n";
        }
        else if (userInput == "p")
        {
            cout << "재생/일시정지 기능은 아직 구현되지 않았습니다.\n";
        }
        else if (userInput == "next")
        {
            cout << "다음 곡 재생 기능은 아직 구현되지 않았습니다.\n";
        }
        else
        {
            cout << "알 수 없는 명령입니다. 다시 입력하세요.\n";
        }

        // 화면을 다시 출력
        displayMenu(currentSong);
    }

    return 0;
}
