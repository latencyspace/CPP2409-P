#include "define.h"
#include "files.h"
#include <algorithm>
#include <random>

class run
{
private:
    char input;
    bool isPaused = false;
    bool isRepeat = false;
    int currentSongIndex = -1;
    vector<int> playlist;
    vector<int> shuffledList;
    bool isShuffle = false;

    void border()
    {
        clearScreen;
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
        clearScreen;
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
                addToPlaylist(currentSongIndex);
                playSong();
            }
        }
        else if (choice == 0)
        {
            cout << "프로그램을 종료합니다." << endl;
            exit(0);
        }
    }

    void showPlaylist()
    {
        clearScreen;
                horizontalFill
                    cout
            << "*               현재 재생 목록              *" << endl;
        horizontalFill

            if (playlist.empty())
        {
            cout << "재생 목록이 비어 있습니다." << endl;
        }
        else
        {
            for (size_t i = 0; i < playlist.size(); ++i)
            {
                int songIndex = playlist[i];
                cout << i + 1 << ". " << getSongTitle(songIndex) << " (" << getSongArtist(songIndex)
                     << ") - " << getSongDuration(songIndex) << endl;
            }
        }

        cout << "\n[B] 메뉴로 돌아가기" << endl;
        cout << "\n[D] 노래 삭제    [C] 목록 초기화    [B] 메뉴로 돌아가기" << endl;

        while (true)
        {
            input = getchar();
            if (input == 'B' || input == 'b')
            if (input == 'D' || input == 'd')
            {
                deleteFromPlaylist();
                return;
            }
            else if (input == 'C' || input == 'c')
            {
                clearPlaylist();
                return;
            }
            else if (input == 'B' || input == 'b')
            {
                return;
            }
        }
    }

    void playSong()
    {
        clearScreen;
                horizontalFill
                    cout
            << "*               현재 재생 중:               *" << endl;
        horizontalFill
                cout
            << "제목: " << getSongTitle(currentSongIndex) << endl;
        cout << "아티스트: " << getSongArtist(currentSongIndex) << endl;
        cout << "길이: " << getSongDuration(currentSongIndex) << endl;
        cout << "\n[SPACE] 일시 정지 / 재생    [B] 메뉴로 돌아가기" << endl;
        cout << "[N] 다음 곡    [P] 이전 곡    [S] 셔플    [R] 반복 재생" << endl;
        cout << "[V] 재생 목록 보기" << endl;

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
            else if (input == 'N' || input == 'n')
            {
                nextSong();
            }
            else if (input == 'P' || input == 'p')
            {
                previousSong();
            }
            else if (input == 'S' || input == 's')
            {
                shufflePlaylist();
            }
            else if (input == 'R' || input == 'r')
            {
                toggleRepeat();
            }
            else if (input == 'V' || input == 'v')
            {
                showPlaylist();
            }
        }
    }

    void addToPlaylist(int songIndex)
    {
        if (find(playlist.begin(), playlist.end(), songIndex) == playlist.end())
        {
            playlist.push_back(songIndex);
        }
    }

    void deleteFromPlaylist()
    {
        clearScreen;
                cout
            << "*               재생 목록에서 제거               *" << endl;
        if (playlist.empty())
        {
            cout << "재생 목록이 비어 있어 제거할 수 없습니다." << endl;
            return;
        }
        cout << "제거할 노래 번호를 입력하세요: " << endl;
        int songNumber;
        cin >> songNumber;
        if (songNumber >= 1 && songNumber <= static_cast<int>(playlist.size()))
        {
            playlist.erase(playlist.begin() + (songNumber - 1));
            cout << "선택한 노래가 재생 목록에서 제거되었습니다." << endl;
        }
        else
        {
            cout << "잘못된 입력입니다." << endl;
        }
    }
    void clearPlaylist()
    {
        playlist.clear();
        cout << "재생 목록이 초기화되었습니다." << endl;
    }
    void nextSong()
    {
        if (isShuffle)
        {
            currentSongIndex = shuffledList[(find(shuffledList.begin(), shuffledList.end(), currentSongIndex) - shuffledList.begin() + 1) % shuffledList.size()];
        }
        else if (currentSongIndex < static_cast<int>(playlist.size()))
        {
            currentSongIndex++;
        }
        else
        {
            currentSongIndex = playlist[0];
        }
        playSong();
    }

    void previousSong()
    {
        if (isShuffle)
        {
            currentSongIndex = shuffledList[(find(shuffledList.begin(), shuffledList.end(), currentSongIndex) - shuffledList.begin() - 1 + shuffledList.size()) % shuffledList.size()];
        }
        else if (currentSongIndex > 1)
        {
            currentSongIndex--;
        }
        else
        {
            currentSongIndex = playlist.back();
        }
        playSong();
    }

    void shufflePlaylist()
    {
        isShuffle = !isShuffle;
        if (isShuffle)
        {
            shuffledList = playlist;
            random_device rd;
            mt19937 g(rd());
            shuffle(shuffledList.begin(), shuffledList.end(), g);
            cout << "[셔플 모드 활성화]" << endl;
        }
        else
        {
            cout << "[셔플 모드 비활성화]" << endl;
        }
    }

    void toggleRepeat()
    {
        isRepeat = !isRepeat;
        if (isRepeat)
        {
            cout << "[반복 재생 활성화]" << endl;
        }
        else
        {
            cout << "[반복 재생 비활성화]" << endl;
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