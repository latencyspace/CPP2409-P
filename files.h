#ifndef FILES_LIBRARY
#define FILES_LIBRARY

#include "define.h"
#include <string>
#include <vector>
#include <map>
#include <dirent.h>

using namespace std;

map<int, string> songList;
map<int, string> songArtist;
map<int, string> songTitle;
map<int, string> songDuration;

void tokenize(const string &str, const char delim, vector<string> &out)
{
    size_t start, end = 0;
    while ((start = str.find_first_not_of(delim, end)) != string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

void populateSongDetails()
{
    vector<string> files;
    DIR *dir;
    struct dirent *diread;

    if ((dir = opendir("./songs/")) != nullptr)
    {
        while ((diread = readdir(dir)) != nullptr)
        {
            string filename = diread->d_name;
            if (filename.size() > 3 && filename.substr(filename.size() - 3) == "mp3")
            {
                files.push_back(filename);
            }
        }
        closedir(dir);
    }
    else
    {
        perror("opendir");
    }

    for (size_t i = 0; i < files.size(); ++i)
    {
        songList[i + 1] = files[i];

        vector<string> details;
        tokenize(files[i], '-', details);

        if (details.size() >= 3)
        {
            songArtist[i + 1] = details[0];
            songTitle[i + 1] = details[1];
            songDuration[i + 1] = details[2].substr(0, details[2].find('.'));
        }
    }
}

string getSongTitle(int index)
{
    return songTitle[index];
}

string getSongArtist(int index)
{
    return songArtist[index];
}

string getSongDuration(int index)
{
    return songDuration[index];
}

#endif
