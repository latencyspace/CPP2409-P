#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cstdio>
#include <thread>

// 일단 기능에 대한 항목부터 다시 정리하고 해당 항목들을 채워나가는 형식으로 개발하기

using namespace std;

class AudioPlayer
{
private:
    vector<string> playlist;

public:
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
            // 파일 존재 여부 확인
            if (ifstream(filename))
            {
                // 파일 삭제
                if (remove(filename.c_str()) == 0) // 파일 삭제 성공 시
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

            // 플레이리스트에서 항목 제거
            playlist.erase(it);
            cout << "다음 파일이 플레이리스트에서 제거되었습니다: " << filename << endl;
        }
        else
        {
            cout << "다음 파일을 플레이리스트에서 찾지 못했습니다: " << filename << endl;
        }
    }

    void showPlaylist() const
    {
        cout << "현재 플레이리스트:" << endl;
        for (const auto &file : playlist)
        {
            cout << file << endl;
        }
    }

    void playPlaylist() const
    {
        for (const auto &file : playlist)
        {
            cout << "재생 중: " << file << endl;
            string command = "ffplay -nodisp -autoexit \"" + file + "\"";
            system(command.c_str());
        }
    }
};

string downloadAudio(const string &url)
{
    // yt-dlp 명령어를 사용하여 MP4 형식으로 다운로드
    string command = "yt-dlp -f 'bestvideo[ext=mp4]+bestaudio[ext=m4a]/best[ext=mp4]' --merge-output-format mp4 -o '%(title)s.%(ext)s' " + url;
    system(command.c_str());

    // 다운로드한 파일 이름 읽기
    string filename = ""; // 파일 이름을 초기화
    string outputFile = "output.txt";
    ifstream infile(outputFile);
    if (infile)
    {
        getline(infile, filename);
    }
    infile.close();

    // output.txt 파일 삭제 (선택 사항)
    remove(outputFile.c_str());

    return filename;
}

int main()
{
    AudioPlayer player;
    string url;

    while (true)
    {
        cout << "1. 플레이리스트 확인하기\n";
        cout << "2. 플레이리스트에 노래 추가하기\n";
        cout << "3. 플레이리스트에 있는 노래 삭제하기\n";
        cout << "4. 플레이리스트 노래 재생하기\n";
        cout << "0. 종료하기\n";
        cout << "원하시는 항목을 선택하여 입력해주세요: ";
        int choice;
        cin >> choice;
        cin.ignore(); // 버퍼 비우기

        if (choice == 0)
            break;

        if (choice == 1)
        {
            player.showPlaylist();
        }
        else if (choice == 2)
        {
            cout << "다운로드할 유튜브 영상의 URL을 입력하세요: ";
            getline(cin, url);

            string filename = downloadAudio(url);
            if (!filename.empty())
            {
                player.addToPlaylist(filename);
                player.showPlaylist();
            }
            else
            {
                cout << "파일 이름을 가져오는 데 실패했습니다." << endl;
            }
        }
        else if (choice == 3)
        {
            cout << "삭제할 노래의 이름을 입력하세요: ";
            string filenameToRemove;
            getline(cin, filenameToRemove);
            player.removeFromPlaylist(filenameToRemove);
            player.showPlaylist();
        }
        else if (choice == 4)
        {
            if (player.showPlaylist().empty())
            {
                cout << "플레이리스트가 비어 있습니다." << endl;
            }
            else
            {
                player.playPlaylist();
            }
        }
    }

    return 0;
}
