#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cstdio>
#include <filesystem>

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
                    cout << "Deleted file: " << filename << endl;
                }
                else
                {
                    cout << "Failed to delete file: " << filename << endl;
                }
            }
            else
            {
                cout << "File not found: " << filename << endl;
            }

            // 플레이리스트에서 항목 제거
            playlist.erase(it);
            cout << "Removed from playlist: " << filename << endl;
        }
        else
        {
            cout << "File not found in playlist: " << filename << endl;
        }
    }

    void showPlaylist() const
    {
        cout << "Current Playlist:" << endl;
        for (const auto &file : playlist)
        {
            cout << file << endl;
        }
    }
};

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

int main()
{
    AudioPlayer player;
    string url;

    while (true)
    {
        cout << "1. 플레이리스트 확인하기\n";
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

            cout << "2. 플레이리스트에 노래 추가하기\n";
            cout << "3. 플레이리스트에 있는 노래 삭제하기\n";
            cout << "원하시는 항목을 선택하여 입력해주세요: ";
            int subChoice;
            cin >> subChoice;
            cin.ignore(); // 버퍼 비우기

            if (subChoice == 2)
            {
                cout << "다운로드할 유튜브 영상의 URL을 입력하세요: ";
                getline(cin, url);

                string command = "yt-dlp " + url;
                int result = system(command.c_str());

                if (result == 0)
                {
                    cout << "다운로드가 완료되었습니다." << endl;
                }
                else
                {
                    cout << "다운로드 중 오류가 발생했습니다." << endl;
                }

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
            else if (subChoice == 3)
            {
                cout << "삭제할 노래의 이름을 입력하세요: ";
                string filenameToRemove;
                getline(cin, filenameToRemove);
                player.removeFromPlaylist(filenameToRemove);
                player.showPlaylist();
            }
        }
    }

    return 0;
}
