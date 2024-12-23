// /usr/bin/g++ -std=c++14 -fdiagnostics-color=always -g /Users/latency/Desktop/Dev/C++/CPP2409-P/test/test.cpp -o /Users/latency/Desktop/Dev/C++/CPP2409-P/test/test

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <cstdlib>

using namespace std;

bool isInstalled(const string &package)
{
#ifdef __linux__
    string command = "dpkg -l | grep " + package;
#elif defined(__APPLE__)
    string command = "brew list | grep " + package;
#elif defined(_WIN32)
    // Windows에서는 설치 여부 확인을 위한 간단한 방법이 제한적입니다.
    return false; // Windows에서는 항상 false를 반환
#else
    return false; // 지원하지 않는 운영 체제
#endif
    return system(command.c_str()) == 0;
}

void installPackage(const string &installCommand)
{
    cout << "설치 중: " << installCommand << endl;
    system(installCommand.c_str());
}

void checkAndInstallDependencies()
{
#ifdef __linux__
    // Ubuntu/Debian 계열
    if (!isInstalled("yt-dlp"))
    {
        installPackage("sudo apt update");
        installPackage("sudo apt install -y yt-dlp");
    }
    if (!isInstalled("mpv"))
    {
        installPackage("sudo apt install -y mpv");
    }
#elif defined(__APPLE__)
    // macOS
    if (!isInstalled("yt-dlp"))
    {
        installPackage("brew install yt-dlp");
    }
    if (!isInstalled("mpv"))
    {
        installPackage("brew install mpv");
    }
#elif defined(_WIN32)
    cout << "Windows에서는 yt-dlp와 mpv를 수동으로 설치해야 합니다." << endl;
    cout << "https://github.com/yt-dlp/yt-dlp#installation 에서 설치 방법을 확인하세요." << endl;
    exit(1);
#else
    cout << "지원하지 않는 운영 체제입니다." << endl;
    exit(1);
#endif
}

void saveLink(const string &link)
{
    ofstream file("Playlist.txt", ios::app);
    if (file.is_open())
    {
        file.seekp(0, ios::end);
        if (file.tellp() != 0)
        {
            file << endl;
        }
        file << link;
        file.close();
    }
}

vector<string> loadPlaylist()
{
    vector<string> playlist;
    ifstream file("Playlist.txt");
    string line;
    while (getline(file, line))
    {
        playlist.push_back(line);
    }
    return playlist;
}

void displayMenu(const string &nowPlaying, const string &playtime)
{
    system("clear"); // Linux/MacOS
    cout << "Now playing: " << nowPlaying << endl;
    cout << "Playtime: " << playtime << endl;
    cout << "1. 노래 재생/일시정지" << endl;
    cout << "2. 플레이리스트 확인" << endl;
    cout << "3. 노래 추가" << endl;
    cout << "4. 노래 삭제" << endl;
    cout << "0. 프로그램 종료" << endl;
}

void playSong(const string &song)
{
    cout << "Now playing: " << song << endl;
    string command = "yt-dlp -x --audio-format mp3 " + song + " --exec \"mpv {}\"";
    system(command.c_str());
}

void deleteSong(vector<string> &playlist)
{
    cout << "현재 플레이리스트:" << endl;
    for (size_t i = 0; i < playlist.size(); i++)
    {
        cout << i + 1 << ". " << playlist[i] << endl;
    }

    int choice;
    cout << "삭제할 노래 번호를 입력하세요: ";
    cin >> choice;
    if (choice > 0 && choice <= playlist.size())
    {
        cout << "해당 노래를 제거하시겠습니까? (Y/N): ";
        char confirm;
        cin >> confirm;
        if (confirm == 'Y' || confirm == 'y')
        {
            playlist.erase(playlist.begin() + (choice - 1));
            ofstream file("Playlist.txt");
            for (const auto &link : playlist)
            {
                file << link << endl;
            }
            file.close();
            cout << "노래가 제거되었습니다." << endl;
        }
    }
}

int main()
{
    checkAndInstallDependencies(); // 의존성 확인 및 설치

    vector<string> playlist;
    string nowPlaying = "없음";
    string playtime = "00:00 / 00:00";

    while (true)
    {
        displayMenu(nowPlaying, playtime);
        int choice;
        cout << "선택하세요: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            if (!playlist.empty())
            {
                playSong(playlist[0]);
            }
            else
            {
                cout << "플레이리스트에 노래가 없습니다." << endl;
            }
            break;
        case 2:
            playlist = loadPlaylist();
            cout << "현재 플레이리스트:" << endl;
            for (size_t i = 0; i < playlist.size(); i++)
            {
                cout << i + 1 << ". " << playlist[i] << endl;
            }
            cin.ignore();
            cin.get(); // 대기
            break;
        case 3:
        {
            cout << "유튜브 링크를 입력하세요: ";
            string link; // link 변수를 여기에 선언
            cin.ignore();
            getline(cin, link);
            saveLink(link);
            cout << "노래가 추가되었습니다." << endl;
            break;
        }
        case 4:
            playlist = loadPlaylist();
            deleteSong(playlist);
            break;
        case 0:
            return 0;
        default:
            cout << "잘못된 선택입니다." << endl;
            break;
        }
    }
}
