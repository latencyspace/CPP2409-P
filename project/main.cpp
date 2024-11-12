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
    // yt-dlp 명령어를 사용하여 오디오 다운로드
    string command = "yt-dlp -x --audio-format mp3 -o '%(title)s.%(ext)s' --get-filename " + url + " > output.txt";
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
