#include "define.h"
#include "files.h"

class run
{
private:
    char input;

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
            << "*              사용 가능한 노래 목록               *" << endl;
        horizontalFill

            vector<string>
                files = getFileList();
        if (files.empty())
        {
            cout << "현재 './songs/' 디렉토리에 노래가 없습니다." << endl;
        }
        else
        {
            for (size_t i = 0; i < files.size(); ++i)
            {
                cout << i + 1 << ". " << files[i] << " (MP3 파일)" << endl;
            }
        }

        cout << "\n계속하려면 아무 키나 누르세요..." << endl;
        getchar();
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
