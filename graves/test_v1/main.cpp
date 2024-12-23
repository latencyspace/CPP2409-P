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
#include <thread>
#include <atomic>

using namespace std;

volatile sig_atomic_t resize_flag = 0;

void handleResize(int sig)
{
    resize_flag = 1; // 신호가 발생하면 플래그를 설정
}

class AudioPlayer
{
private:
    vector<string> playlist;
    atomic<bool> isPlaying; // 현재 재생 상태를 저장

public:
    AudioPlayer() : isPlaying(false) {}

    void addToPlaylist(const string &filename)
    {
        playlist.push_back(filename);
        cout << "Added to playlist: " << filename << endl;
    }

    void removeFromPlaylist(const string &filename)
    {
        auto it = find(playlist.begin(), playlist.end(), filename);
        if (it != playlist.end())
        {
            if (ifstream(filename))
            {
                if (remove(filename.c_str()) == 0)
                {
                    cout << "다음 파일을 삭제하였습니다: " << filename << endl;
                }
                else
                {
                    cout << "다음 파일 삭제에 실패했습니다: " << filename << endl;
                }
            }
            else
            {
                cout << "다음 파일을 찾을 수 없습니다: " << filename << endl;
            }

            playlist.erase(it);
            cout << "다음 파일이 플레이리스트에서 제거되었습니다: " << filename << endl;
        }
        else
        {
            cout << "다음 파일을 플레이리스트에서 찾지 못했습니다: " << filename << endl;
        }
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
        cout << "4. 플레이리스트 확인\n";
        cout << "5. 노래 추가\n";
        cout << "6. 노래 삭제\n";
        cout << "0. 프로그램 종료\n";
        cout << line << "\n";
        cout << "무엇을 실행하시겠어요?: ";
    }

    void displayPlaylist()
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
            cout << i + 1 << ". " << playlist[i] << "\n";
        }
        cout << line << "\n";
    }

    string downloadAudio(const string &url)
    {
        string command = "yt-dlp -f bestvideo[ext=mp4]+bestaudio[ext=m4a]/best[ext=mp4] -o '%(title)s.%(ext)s' --get-filename " + url + " > output.txt";
        system(command.c_str());

        ifstream infile("output.txt");
        string filename;
        if (infile)
        {
            getline(infile, filename);
        }
        infile.close();

        remove("output.txt");

        return filename;
    }

    void playSong(const string &filename, string &currentSong)
    {
        currentSong = filename;
        isPlaying = true;

        string command = "mpg123 \"" + filename + "\"";
        int result = system(command.c_str());

        if (result != 0)
        {
            cout << "노래 재생에 실패했습니다: " << filename << "\n";
            isPlaying = false;
        }
    }

    void togglePlayPause(string &currentSong)
    {
        if (isPlaying)
        {
            cout << "일시정지합니다: " << currentSong << "\n";
            isPlaying = false;
        }
        else
        {
            cout << "노래를 재생합니다: " << currentSong << "\n";
            playSong(currentSong, currentSong);
        }
    }

    void installHomebrew()
    {
        int result = system("command -v brew");
        if (result != 0)
        {
            cout << "Homebrew가 설치되어 있지 않습니다. 설치 중...\n";
            system("/bin/bash -c \"$(curl -fsSL \"");
        }
    }

    void installMpg123()
    {
        int result = system("command -v mpg123");
        if (result != 0)
        {
            cout << "mpg123가 설치되어 있지 않습니다. 설치 중...\n";
            system("brew install mpg123");
        }
    }

    void setupEnvironment()
    {
        system("python3 -m venv path/to/venv");
        system("source path/to/venv/bin/activate");
        system("path/to/venv/bin/pip install yt-dlp");
        installHomebrew();
        installMpg123();
    }
};

int main()
{
    AudioPlayer player;
    player.setupEnvironment();

    string url;
    string currentSong = "";
    string userInput;

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handleResize;
    sigaction(SIGWINCH, &sa, nullptr);

    player.displayMenu(currentSong);

    while (true)
    {
        if (resize_flag)
        {
            resize_flag = 0;
            player.displayMenu(currentSong);
        }

        getline(cin, userInput);

        if (userInput == "1")
        {
            cout << "이전 곡 기능 실행\n";
        }
        else if (userInput == "2")
        {
            if (!currentSong.empty())
            {
                player.togglePlayPause(currentSong);
            }
            else
            {
                cout << "현재 재생 중인 곡이 없습니다.\n";
            }
        }
        else if (userInput == "3")
        {
            cout << "다음 곡 기능 실행\n";
        }
        else if (userInput == "4")
        {
            player.displayPlaylist();
            cout << "계속하려면 Enter 키를 누르세요...\n";
            cin.ignore();
        }
        else if (userInput == "5")
        {
            cout << "다운로드 받을 음원의 유튜브 URL을 입력하세요: ";
            getline(cin, url);

            string filename = player.downloadAudio(url);
            if (!filename.empty())
            {
                player.addToPlaylist(filename);
            }
            else
            {
                cout << "파일 이름을 가져오는 데 실패했습니다." << endl;
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

        player.displayMenu(currentSong);
    }

    return 0;
}
