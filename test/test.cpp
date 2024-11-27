// /usr/bin/g++ -std=c++14 -fdiagnostics-color=always -g /Users/latency/Desktop/Dev/C++/CPP2409-P/test/test.cpp -o /Users/latency/Desktop/Dev/C++/CPP2409-P/test/test

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

atomic<bool> isPlaying(false);
string nowPlaying = "없음";        // 전역 변수로 선언
string playtime = "00:00 / 00:00"; // 전역 변수로 선언

void checkAndInstallDependencies()
{
    // 시스템에 따라 적절한 패키지 관리자를 사용하여 yt-dlp와 mpv를 설치합니다.
#ifdef __linux__
    system("sudo apt update");
    system("sudo apt install -y yt-dlp mpv");
#elif defined(__APPLE__)
    system("brew install yt-dlp mpv");
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
    isPlaying = true;  // 노래 재생 상태 설정
    nowPlaying = song; // 현재 재생 중인 노래 정보 업데이트
    string command = "yt-dlp -x --audio-format mp3 " + song + " --exec \"mpv {}\"";
    system(command.c_str());
    isPlaying = false; // 노래 재생이 끝나면 상태 변경
}

void songPlayerThread(vector<string> &playlist)
{
    for (const auto &song : playlist)
    {
        playSong(song);
        // 노래가 끝난 후 대기
        if (isPlaying)
        {
            cout << "다음 노래로 넘어갑니다..." << endl;
            std::this_thread::sleep_for(std::chrono::seconds(2)); // 2초 대기
        }
    }
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

    while (true)
    {
        displayMenu(nowPlaying, playtime); // nowPlaying을 사용하여 현재 재생 중인 노래 표시
        int choice;
        cout << "사용하려는 기능에 대한 번호를 입력해주세요: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            playlist = loadPlaylist();
            if (!playlist.empty())
            {
                thread player(songPlayerThread, ref(playlist)); // 노래 재생 스레드 시작
                player.detach();                                // 스레드를 분리하여 메인 스레드와 독립적으로 실행
                // 위 코드로 스레드 분리해서 실행하다보니까 Now playing: & Playtime: 부분이 동적으로 값을 못받아오는 것 같음

                displayMenu(nowPlaying, playtime); // nowPlaying을 사용하여 현재 재생 중인 노래 표시
                int choice;
                cout << "사용하려는 기능에 대한 번호를 입력해주세요: ";
                cin >> choice;
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
            cout << "이전으로 돌아가려면 'Enter'를 입력하세요." << endl;
            cin.ignore();
            cin.get(); // 대기
            break;
        case 3:
        {
            cout << "유튜브 링크를 입력하세요: ";
            string link;
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