#ifndef FILES_LIBRARY
#define FILES_LIBRARY

#include "define.h"
#include <dirent.h>
#include <vector>
#include <string>
#include <map>
using namespace std;

vector<string> getFileList()
{
    vector<string> fileList;
    DIR *dir;
    struct dirent *diread;

    if ((dir = opendir("./songs/")) != nullptr)
    {
        while ((diread = readdir(dir)) != nullptr)
        {
            string filename = diread->d_name;
            if (filename.size() > 3 && filename.substr(filename.size() - 3) == "mp3")
            {
                fileList.push_back(filename);
            }
        }
        closedir(dir);
    }
    else
    {
        perror("opendir");
    }

    return fileList;
}

#endif
