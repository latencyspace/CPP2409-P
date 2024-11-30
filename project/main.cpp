// /usr/bin/g++ -std=c++17 -fdiagnostics-color=always -g /Users/latency/Desktop/Dev/C++/CPP2409-P/project/main.cpp -o /Users/latency/Desktop/Dev/C++/CPP2409-P/project/main

#include "define.h"
#include "files.h"

class run
{
private:
    char input;
    bool isPaused = false;
    int currentSongIndex = -1;

    void border()
    {
        clearScreen
                horizontalFill
                    horizontalBlank
                        cout
            << "*       음악 플레이어에 오신 것을 환영합니다!      *" << endl;
        horizontalBlank
                horizontalFill
                    cout
            << "'L' 키를 눌러 노래 목록을 확인하거나, '스페이스' 키를 눌러 종료하세요." << endl;
    }

    void listSongs()
    {
        clearScreen
                horizontalFill
                    cout
            << "*               사용 가능한 노래 목록              *" << endl;
        horizontalFill

        populateSongDetails();
        if (songList.empty())
        {
            cout << "현재 './songs/' 디렉토리에 노래가 없습니다." << endl;
        }
        else
        {
            for (const auto &[index, file] : songList)
            {
                cout << index << ". " << getSongTitle(index) << " (" << getSongArtist(index)
                     << ") - " << getSongDuration(index) << endl;
            }
        }

        cout << "\n노래를 선택하려면 번호를 입력하세요 (종료: '0')..." << endl;
        int choice;
        cin >> choice;

        if (choice >= 1 && choice <= 9)
        {
            if (songList.find(choice) != songList.end())
            {
                currentSongIndex = choice;
                playSong();
            }
        }
        else if (choice == 0)
        {
            cout << "프로그램을 종료합니다." << endl;
            exit(0);
        }
    }

    void playSong()
    {
        clearScreen
                horizontalFill
                    cout
            << "*                   현재 재생 중:                  *" << endl;
        horizontalFill
                cout
            << "제목: " << getSongTitle(currentSongIndex) << endl;
        cout << "아티스트: " << getSongArtist(currentSongIndex) << endl;
        cout << "길이: " << getSongDuration(currentSongIndex) << endl;
        cout << "\n[SPACE] 일시 정지 / 재생    [B] 메뉴로 돌아가기" << endl;

        handlePlayback();
    }

    void handlePlayback()
    {
        while (true)
        {
            input = getchar();

            if (input == ' ')
            {
                isPaused = !isPaused;
                if (isPaused)
                {
                    cout << "\n[일시 정지됨] 아무 키나 눌러 계속하세요..." << endl;
                }
                else
                {
                    cout << "\n[재생 중] 계속 재생..." << endl;
                }
            }
            else if (input == 'B' || input == 'b')
            {
                isPaused = false;
                return;
            }
        }
    }

public:
    run()
    {
        while (true)
        {
            border();
            input = getchar();
            if (input == ' ')
            {
                clearScreen;
                exit(0);
            }
            else if (input == 'L' || input == 'l')
            {
                listSongs();
            }
        }
    }
};

int main(void)
{
    run MusicPlayer;
    return 0;
}
