// /usr/bin/g++ -std=c++14 -fdiagnostics-color=always -g /Users/latency/Desktop/Dev/C++/CPP2409-P/test/test.cpp -o /Users/latency/Desktop/Dev/C++/CPP2409-P/test/test

#include <iostream>
#include <string>
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <cstdlib>
#include <fstream>

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
    cout << "Now playing: " << currentSong << "\n"; // 현재 재생 중인 음악 제목
    cout << line << "\n";
    cout << "1. 이전 곡\n";
    cout << "2. 재생/일시정지\n";
    cout << "3. 다음 곡\n";
    cout << "4. 플레이리스트 확인\n";
    cout << "5. 노래 추가\n";
    cout << "6. 노래 삭제\n";
    cout << "0. 프로그램 종료\n";
    cout << line << "\n";
    cout << "무엇을 실행하시겠어요?: ";
}

void displayPlaylist(const vector<string> &playlist)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int width = w.ws_col;

    string line(width, '=');

    cout << "\033[H\033[J"; // 화면 지우기
    cout << line << "\n";
    cout << "플레이리스트:\n";
    for (size_t i = 0; i < playlist.size(); ++i)
    {
        cout << i + 1 << ". " << playlist[i] << "\n"; // 플레이리스트 항목 출력
    }
    cout << line << "\n";
}

string downloadAudio(const string &url)
{
    // yt-dlp 명령어를 사용하여 MP4 형식으로 다운로드
    string command = "yt-dlp -f bestvideo[ext=mp4]+bestaudio[ext=m4a]/best[ext=mp4] -o '%(title)s.%(ext)s' --get-filename " + url + " > output.txt";
    system(command.c_str());

    // 다운로드한 파일 이름 읽기
    ifstream infile("output.txt");
    string filename;
    if (infile)
    {
        getline(infile, filename);
    }
    infile.close();

    // output.txt 파일 삭제 (선택 사항)
    remove("output.txt");

    return filename;
}

void playSong(const string &filename, string &currentSong)
{
    // CLI에서 노래 재생
    currentSong = filename; // 현재 재생 중인 음악 제목 업데이트

    // 파일 경로에 공백이 포함될 경우를 대비하여 따옴표로 감싸줍니다.
    string command = "mpg123 \"" + filename + "\""; // mpg123를 사용하여 재생
    int result = system(command.c_str());

    // 재생 실패 시 에러 메시지
    if (result != 0)
    {
        cout << "노래 재생에 실패했습니다: " << filename << "\n";
    }
}

void installHomebrew()
{
    // Homebrew가 설치되어 있는지 확인하고 설치
    int result = system("command -v brew");
    if (result != 0)
    {
        cout << "Homebrew가 설치되어 있지 않습니다. 설치 중...\n";
        system("/bin/bash -c \"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\"");
    }
}

void installMpg123()
{
    // mpg123가 설치되어 있는지 확인하고 설치
    int result = system("command -v mpg123");
    if (result != 0)
    {
        cout << "mpg123가 설치되어 있지 않습니다. 설치 중...\n";
        system("brew install mpg123");
    }
}

void setupEnvironment()
{
    // 가상 환경 생성
    system("python3 -m venv path/to/venv");

    // 가상 환경 활성화 (Linux/macOS)
    system("source path/to/venv/bin/activate");

    // yt-dlp 설치
    system("path/to/venv/bin/pip install yt-dlp");

    installHomebrew(); // Homebrew 설치 확인 및 설치
    installMpg123();   // mpg123 설치 확인 및 설치
}

int main()
{
    setupEnvironment();

    string currentSong = ""; // 현재 재생 중인 음악 제목
    string userInput;
    vector<string> playlist; // 플레이리스트를 저장할 벡터

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
            // 재생/일시정지 기능 실행
            cout << "현재 재생 중인 노래: " << currentSong << "\n";
            if (!currentSong.empty())
            {
                cout << "노래를 일시정지하려면 'p', 재생하려면 'r'을 입력하세요: ";
                char action;
                cin >> action;
                cin.ignore(); // 남은 개행 문자 무시
                if (action == 'p')
                {
                    // 일시정지 로직 (여기서는 단순히 출력)
                    cout << "노래가 일시정지되었습니다.\n";
                }
                else if (action == 'r')
                {
                    // 재생 로직 (여기서는 단순히 출력)
                    cout << "노래가 재생됩니다.\n";
                    playSong(currentSong, currentSong); // 현재 노래 다시 재생
                }
                else
                {
                    cout << "잘못된 입력입니다.\n";
                }
            }
            else
            {
                cout << "현재 재생 중인 노래가 없습니다.\n";
            }
        }
        else if (userInput == "3")
        {
            cout << "다음 곡 기능 실행\n";
        }
        else if (userInput == "4")
        {
            // 플레이리스트 확인
            displayPlaylist(playlist); // 플레이리스트 출력
            cout << "계속하려면 Enter 키를 누르세요...\n";
            cin.ignore(); // Enter 키 대기
        }
        else if (userInput == "5")
        {
            // 노래 추가
            cout << "다운로드할 유튜브 영상의 URL을 입력하세요: ";
            string url;
            getline(cin, url);

            // yt-dlp 명령어로 다운로드
            string command = "yt-dlp " + url;
            int result = system(command.c_str());

            if (result == 0)
            {
                cout << "다운로드가 완료되었습니다." << endl;

                // 다운로드 후 파일 이름 가져오기
                string filename = downloadAudio(url);
                if (!filename.empty())
                {
                    playlist.push_back(filename); // 다운로드한 파일명을 플레이리스트에 추가
                    cout << "노래가 추가되었습니다: " << filename << "\n";
                    playSong(filename, currentSong); // 다운로드 후 바로 재생
                }
                else
                {
                    cout << "파일 이름을 가져오는 데 실패했습니다." << endl;
                }
            }
            else
            {
                cout << "다운로드 중 오류가 발생했습니다." << endl;
            }
        }
        else if (userInput == "6")
        {
            // 노래 삭제
            cout << "삭제할 노래 번호를 입력하세요: ";
            int songNumber;
            cin >> songNumber;
            cin.ignore(); // 남은 개행 문자 무시
            if (songNumber > 0 && songNumber <= playlist.size())
            {
                cout << "노래가 삭제되었습니다: " << playlist[songNumber - 1] << "\n";
                playlist.erase(playlist.begin() + (songNumber - 1)); // 노래 삭제
            }
            else
            {
                cout << "잘못된 번호입니다.\n";
            }
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
