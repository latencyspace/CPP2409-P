#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

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

    return 0;
}
