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
        cout << "*       Welcome to the Music Player!       *" << endl;
        horizontalBlank
        horizontalFill
        cout << "'Press 'L' to view the song list or 'SPACE' to exit." << endl;
    }

    void listSongs()
{
    clearScreen;
    horizontalFill
    cout << "*               Available Song List              *" << endl;
    horizontalFill

    populateSongDetails(); // Populate song list from the 'songs' folder
    if (songList.empty())
    {
        cout << "There are currently no songs in the './songs/' directory." << endl;
    }
    else
    {
        for (const auto& [index, file] : songList)
        {
            cout << index << ". " << getSongTitle(index) << " (" << getSongArtist(index)
                 << ") - " << getSongDuration(index) << endl;
        }
    }

    cout << "\nEnter the number of the song you want to select (exit: '0')..." << endl;
    int choice;
    cin >> choice;

    if (choice >= 1 && choice <= static_cast<int>(songList.size()))
    {
        if (songList.find(choice) != songList.end())
        {
            string selectedSongPath = "./songs/" + songList[choice];
            playSong(selectedSongPath); // Play the selected song
        }
    }
    else if (choice == 0)
    {
        cout << "Exiting the program." << endl;
        exit(0);
    }
    else
    {
        cout << "Invalid selection. Returning to the main menu." << endl;
    }
}


    void showPlaylist()
    {
        clearScreen;
        horizontalFill
        cout << "*               Current Playlist              *" << endl;
        horizontalFill

        if (playlist.empty())
        {
            cout << "The playlist is empty." << endl;
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

        cout << "\n[B] Return to menu" << endl;
        cout << "\n[D] Remove song    [C] Clear playlist    [B] Return to menu" << endl;

        while (true)
        {
            input = getchar();
            if (input == 'B' || input == 'b')
            {
                return;
            }
            else if (input == 'D' || input == 'd')
            {
                deleteFromPlaylist();
                return;
            }
            else if (input == 'C' || input == 'c')
            {
                clearPlaylist();
                return;
            }
        }
    }

    void playSong(const string& relativePath)
    {
        clearScreen;
        horizontalFill
        cout << "*               Currently Playing:               *" << endl;
        horizontalFill
        cout << "Title: " << getSongTitle(currentSongIndex) << endl;
        cout << "Artist: " << getSongArtist(currentSongIndex) << endl;
        cout << "Duration: " << getSongDuration(currentSongIndex) << endl;

    // Convert relative path to absolute path
    string absolutePath = fs::absolute(relativePath).string();

    // Construct the command to play the song using FFplay
    string command = "ffplay -nodisp -autoexit -loglevel quiet \"" + absolutePath + "\"";

    // Execute the command to play the song
    int result = system(command.c_str());
    if (result != 0)
    {
        cerr << "Error: Failed to play the song." << endl;
        cerr << "Ensure that FFmpeg (ffplay) is installed and available in the system PATH." << endl;
    }

    if (result != 0)
    {
        cerr << "Error: Failed to play the song." << endl;
    }

        cout << "\n[SPACE] Pause / Play    [B] Return to menu" << endl;
        cout << "[N] Next song    [P] Previous song    [S] Shuffle    [R] Repeat" << endl;
        cout << "[V] View playlist" << endl;

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
                    cout << "\n[Paused] Press any key to continue..." << endl;
                }
                else
                {
                    cout << "\n[Playing] Continuing playback..." << endl;
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
        cout << "*               Remove from Playlist               *" << endl;
        if (playlist.empty())
        {
            cout << "The playlist is empty and cannot be removed." << endl;
            return;
        }
        cout << "Enter the number of the song to remove: " << endl;
        int songNumber;
        cin >> songNumber;
        if (songNumber >= 1 && songNumber <= static_cast<int>(playlist.size()))
        {
            playlist.erase(playlist.begin() + (songNumber - 1));
            cout << "The selected song has been removed from the playlist." << endl;
        }
        else
        {
            cout << "Invalid input." << endl;
        }
    }

    void clearPlaylist()
    {
        playlist.clear();
        cout << "The playlist has been cleared." << endl;
    }

    void nextSong()
{
    if (isShuffle)
    {
        // Shuffle mode: Move to the next song in shuffledList
        currentSongIndex = shuffledList[(find(shuffledList.begin(), shuffledList.end(), currentSongIndex) - shuffledList.begin() + 1) % shuffledList.size()];
    }
    else
    {
        // Non-shuffle mode: Increment index or loop back to the start
        auto it = find(playlist.begin(), playlist.end(), currentSongIndex);
        if (it != playlist.end() && ++it != playlist.end())
        {
            currentSongIndex = *it;
        }
        else
        {
            currentSongIndex = playlist[0]; // Loop back to the first song
        }
    }

    // Play the next song
    string nextSongPath = "./songs/" + songList[currentSongIndex];
    playSong(nextSongPath);
}

    void previousSong()
{
    if (isShuffle)
    {
        // Shuffle mode: Move to the previous song in shuffledList
        currentSongIndex = shuffledList[(find(shuffledList.begin(), shuffledList.end(), currentSongIndex) - shuffledList.begin() - 1 + shuffledList.size()) % shuffledList.size()];
    }
    else
    {
        // Non-shuffle mode: Decrement index or loop back to the end
        auto it = find(playlist.begin(), playlist.end(), currentSongIndex);
        if (it != playlist.begin())
        {
            currentSongIndex = *(--it);
        }
        else
        {
            currentSongIndex = playlist.back(); // Loop back to the last song
        }
    }

    // Play the previous song
    string prevSongPath = "./songs/" + songList[currentSongIndex];
    playSong(prevSongPath);
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
            cout << "[Shuffle mode enabled]" << endl;
        }
        else
        {
            cout << "[Shuffle mode disabled]" << endl;
        }
    }

    void toggleRepeat()
    {
        isRepeat = !isRepeat;
        if (isRepeat)
        {
            cout << "[Repeat mode enabled]" << endl;
        }
        else
        {
            cout << "[Repeat mode disabled]" << endl;
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