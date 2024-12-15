#include "define.h"  // 필요한 상수를 정의한 헤더 파일 포함
#include "files.h"   // 파일 관련 기능을 제공하는 헤더 파일 포함
#include <algorithm>  // 알고리즘 관련 함수 사용을 위해 포함
#include <random>     // 난수 생성을 위해 포함
#include <thread>     // 멀티스레드 작업을 위해 포함

class run
{
private:
    char input;  // 사용자 입력을 저장하는 변수
    bool isPaused = false;  // 재생/일시 정지 상태를 나타내는 변수
    bool isRepeat = false;  // 반복 재생 모드 여부를 나타내는 변수
    int currentSongIndex = -1;  // 현재 재생 중인 곡의 인덱스
    vector<int> playlist;  // 재생 목록을 저장하는 벡터
    vector<int> shuffledList;  // 셔플된 재생 목록을 저장하는 벡터
    bool isShuffle = false;  // 셔플 모드 활성화 여부

    void border() // 화면 경계를 출력하는 함수
    {
        clearScreen;  // 화면을 초기화
        horizontalFill // 화면 상단 경계 출력
        horizontalBlank // 화면 빈 줄 출력
        cout << "*           Welcome to the Music Player!           *" << endl; // 환영 메시지 출력
        horizontalBlank // 화면 빈 줄 출력
        horizontalFill // 화면 하단 경계 출력
        cout << "'Press 'L' to view the song list or 'SPACE' to exit." << endl; // 사용자가 할 수 있는 작업 안내
    }

    void listSongs() // 노래 목록을 표시하고 선택하는 함수
    {
        clearScreen;  // 화면 초기화
        horizontalFill // 화면 상단 경계 출력
        cout << "*                Available Song List               *" << endl; // 노래 목록 제목 출력
        horizontalFill // 화면 하단 경계 출력

        populateSongDetails(); // 'songs' 폴더에서 노래 목록을 불러오는 함수 호출
        if (songList.empty()) // 노래 목록이 비어 있는 경우
        {
            cout << "There are currently no songs in the './songs/' directory." << endl; // 노래가 없음을 알림
        }
        else // 노래가 있는 경우
        {
            for (const auto& [index, file] : songList) // map에 저장된 노래 목록을 출력
            {
                cout << index << ". " << getSongTitle(index) << " (" << getSongArtist(index)
                     << ") - " << getSongDuration(index) << endl; // 인덱스, 제목, 아티스트, 재생 시간 출력
            }
        }

        cout << "\nEnter the number of the song you want to select (exit: '0')..." << endl; // 사용자 입력 안내
        int choice;
        cin >> choice; // 사용자 입력 받기

        if (choice >= 1 && choice <= static_cast<int>(songList.size())) // 유효한 노래 인덱스를 선택한 경우
        {
            if (songList.find(choice) != songList.end()) // 선택한 노래가 목록에 있는지 확인
            {
                string selectedSongPath = "./songs/" + songList[choice]; // 선택한 노래의 경로 생성
                playSong(selectedSongPath); // 선택한 노래 재생
            }
        }
        else if (choice == 0) // 사용자가 0을 선택한 경우
        {
            cout << "Exiting the program." << endl; // 프로그램 종료 메시지 출력
            exit(0); // 프로그램 종료
        }
        else // 유효하지 않은 선택인 경우
        {
            cout << "Invalid selection. Returning to the main menu." << endl; // 잘못된 입력 알림
        }
    }

    void showPlaylist() // 현재 재생 목록을 출력하는 함수
    {
        clearScreen; // 화면 초기화
        horizontalFill // 화면 상단 경계 출력
        cout << "*                Current Playlist               *" << endl; // 재생 목록 제목 출력
        horizontalFill // 화면 하단 경계 출력

        if (playlist.empty()) // 재생 목록이 비어 있는 경우
        {
            cout << "The playlist is empty." << endl; // 빈 목록 알림
        }
        else // 재생 목록이 있는 경우
        {
            for (size_t i = 0; i < playlist.size(); ++i) // 재생 목록의 각 노래를 출력
            {
                int songIndex = playlist[i]; // 현재 곡의 인덱스
                cout << i + 1 << ". " << getSongTitle(songIndex) << " (" << getSongArtist(songIndex)
                     << ") - " << getSongDuration(songIndex) << endl; // 제목, 아티스트, 재생 시간 출력
            }
        }

        cout << "\n[B] Return to menu" << endl; // 메뉴로 돌아가기 옵션 출력
        cout << "\n[D] Remove song    [C] Clear playlist    [B] Return to menu" << endl; // 추가 옵션 출력

        while (true) // 사용자 입력 대기 루프
        {
            input = getchar(); // 사용자 입력 받기
            if (input == 'B' || input == 'b') // 메뉴로 돌아가기
            {
                return; // 함수 종료
            }
            else if (input == 'D' || input == 'd') // 노래 삭제
            {
                deleteFromPlaylist(); // 삭제 함수 호출
                return; // 함수 종료
            }
            else if (input == 'C' || input == 'c') // 재생 목록 초기화
            {
                clearPlaylist(); // 초기화 함수 호출
                return; // 함수 종료
            }
        }
    }

    void playSong(const string& relativePath) // 노래를 재생하는 함수
    {
        string absolutePath = fs::absolute(relativePath).string(); // 상대 경로를 절대 경로로 변환

        auto it = find_if(songList.begin(), songList.end(), [&](const pair<int, string>& entry) {
            return relativePath.find(entry.second) != string::npos;
        }); // 노래 인덱스 검색

        int songIndex = (it != songList.end()) ? it->first : -1; // 검색된 노래의 인덱스 가져오기

        if (songIndex != -1) // 노래가 존재하는 경우
        {
            clearScreen; // 화면 초기화
            horizontalFill // 화면 상단 경계 출력
            cout << "*               Currently Playing:               *" << endl; // 현재 재생 중 메시지
            horizontalFill // 화면 하단 경계 출력
            cout << "Title: " << getSongTitle(songIndex) << endl; // 제목 출력
            cout << "Artist: " << getSongArtist(songIndex) << endl; // 아티스트 출력
            cout << "Duration: " << getSongDuration(songIndex) << endl; // 재생 시간 출력
            horizontalFill // 경계 출력

            string command = "ffplay -nodisp -autoexit -loglevel quiet \"" + absolutePath + "\""; // ffplay 명령어 생성
            int result = system(command.c_str()); // 노래 재생 명령 실행
            if (result != 0) // 오류 발생 시
            {
                cerr << "Error: Failed to play the song." << endl; // 오류 메시지 출력
                cerr << "Ensure that FFmpeg (ffplay) is installed and available in the system PATH." << endl; // 해결 방법 제시
            }
        }
        else // 노래가 없는 경우
        {
            cerr << "Error: Song not found in the list." << endl; // 오류 메시지 출력
        }
    }


void playSongInBackground(const string& absolutePath) // 백그라운드에서 노래를 재생하는 함수
    {
        string command = "ffplay -nodisp -autoexit -loglevel quiet \"" + absolutePath + "\""; // ffplay 명령어 생성
        int result = system(command.c_str()); // 노래 재생 명령 실행
        if (result != 0) // 오류 발생 시
        {
            cerr << "Error: Failed to play the song." << endl; // 오류 메시지 출력
            cerr << "Ensure that FFmpeg (ffplay) is installed and available in the system PATH." << endl; // 해결 방법 제시
        }
    }

    void handlePlayback() // 재생 중 사용자 입력 처리 함수
    {
        char input; // 사용자 입력을 저장하는 변수
        while (true) // 무한 루프
        {
            input = getchar(); // 사용자 입력 받기
            if (input == ' ') // 재생/일시 정지 토글
            {
                cout << "[Paused] Press any key to continue..." << endl; // 일시 정지 상태 알림
                getchar(); // 입력 대기
                cout << "[Playing] Continuing playback..." << endl; // 재생 상태 알림
            }
            else if (input == 'B' || input == 'b') // 메뉴로 돌아가기
            {
                cout << "Returning to menu..." << endl; // 알림 출력
                break; // 루프 종료
            }
            else if (input == 'N' || input == 'n') // 다음 곡으로 이동
            {
                cout << "Playing next song..." << endl; // 알림 출력
            }
            else if (input == 'P' || input == 'p') // 이전 곡으로 이동
            {
                cout << "Playing previous song..." << endl; // 알림 출력
            }
            else if (input == 'S' || input == 's') // 셔플 모드 활성화/비활성화
            {
                cout << "Shuffling playlist..." << endl; // 알림 출력
            }
            else if (input == 'R' || input == 'r') // 반복 모드 활성화/비활성화
            {
                cout << "Repeating song..." << endl; // 알림 출력
            }
            else if (input == 'V' || input == 'v') // 재생 목록 보기
            {
                cout << "Viewing playlist..." << endl; // 알림 출력
            }
        }
    }

void playSongWithInput(const string& relativePath) // 사용자 입력과 함께 노래를 재생하는 함수
    {
        string absolutePath = fs::absolute(relativePath).string(); // 상대 경로를 절대 경로로 변환

        auto it = find_if(songList.begin(), songList.end(), [&](const pair<int, string>& entry) {
            return relativePath.find(entry.second) != string::npos;
        }); // 노래 인덱스 검색

        int songIndex = (it != songList.end()) ? it->first : -1; // 검색된 노래의 인덱스 가져오기

        if (songIndex != -1) // 노래가 존재하는 경우
        {
            clearScreen; // 화면 초기화
            horizontalFill // 화면 상단 경계 출력
            cout << "*               Currently Playing:               *" << endl; // 현재 재생 중 메시지
            horizontalFill // 화면 하단 경계 출력
            cout << "Title: " << getSongTitle(songIndex) << endl; // 제목 출력
            cout << "Artist: " << getSongArtist(songIndex) << endl; // 아티스트 출력
            cout << "Duration: " << getSongDuration(songIndex) << endl; // 재생 시간 출력
            horizontalFill // 경계 출력

            thread playThread(&run::playSongInBackground, this, absolutePath); // 백그라운드에서 노래 재생
            handlePlayback(); // 사용자 입력 처리
            playThread.join(); // 백그라운드 스레드 종료 대기
        }
        else // 노래가 없는 경우
        {
            cerr << "Error: Song not found in the list." << endl; // 오류 메시지 출력
        }
    }


    void addToPlaylist(int songIndex) // 재생 목록에 노래를 추가하는 함수
    {
        if (find(playlist.begin(), playlist.end(), songIndex) == playlist.end()) // 목록에 중복 없이 추가
        {
            playlist.push_back(songIndex); // 노래 인덱스를 목록에 추가
        }
    }

    void deleteFromPlaylist() // 재생 목록에서 노래를 삭제하는 함수
    {
        clearScreen; // 화면 초기화
        cout << "*               Remove from Playlist               *" << endl; // 삭제 메뉴 출력
        if (playlist.empty()) // 목록이 비어 있는 경우
        {
            cout << "The playlist is empty and cannot be removed." << endl; // 삭제 불가 메시지 출력
            return; // 함수 종료
        }
        cout << "Enter the number of the song to remove: " << endl; // 사용자 입력 안내
        int songNumber;
        cin >> songNumber; // 사용자 입력 받기
        if (songNumber >= 1 && songNumber <= static_cast<int>(playlist.size())) // 유효한 인덱스인 경우
        {
            playlist.erase(playlist.begin() + (songNumber - 1)); // 해당 곡 삭제
            cout << "The selected song has been removed from the playlist." << endl; // 삭제 완료 메시지
        }
        else // 유효하지 않은 입력인 경우
        {
            cout << "Invalid input." << endl; // 오류 메시지 출력
        }
    }

    void clearPlaylist() // 재생 목록을 초기화하는 함수
    {
        playlist.clear(); // 재생 목록 비우기
        cout << "The playlist has been cleared." << endl; // 초기화 완료 메시지
    }
    void nextSong() // 다음 곡으로 이동하는 함수
    {
        if (isShuffle) // 셔플 모드인 경우
        {
            currentSongIndex = shuffledList[(find(shuffledList.begin(), shuffledList.end(), currentSongIndex) - shuffledList.begin() + 1) % shuffledList.size()]; // 셔플 리스트 기준 이동
        }
        else // 일반 모드인 경우
        {
            auto it = find(playlist.begin(), playlist.end(), currentSongIndex); // 현재 곡 위치 찾기
            if (it != playlist.end() && ++it != playlist.end()) // 다음 곡 존재 여부 확인
            {
                currentSongIndex = *it; // 다음 곡 인덱스 설정
            }
            else
            {
                currentSongIndex = playlist[0]; // 첫 곡으로 순환
            }
        }
        string nextSongPath = "./songs/" + songList[currentSongIndex]; // 다음 곡 경로 생성
        playSong(nextSongPath); // 다음 곡 재생
    }

    void previousSong() // 이전 곡으로 이동하는 함수
    {
        if (isShuffle) // 셔플 모드인 경우
        {
            currentSongIndex = shuffledList[(find(shuffledList.begin(), shuffledList.end(), currentSongIndex) - shuffledList.begin() - 1 + shuffledList.size()) % shuffledList.size()]; // 셔플 리스트 기준 이동
        }
        else // 일반 모드인 경우
        {
            auto it = find(playlist.begin(), playlist.end(), currentSongIndex); // 현재 곡 위치 찾기
            if (it != playlist.begin()) // 이전 곡 존재 여부 확인
            {
                currentSongIndex = *(--it); // 이전 곡 인덱스 설정
            }
            else
            {
                currentSongIndex = playlist.back(); // 마지막 곡으로 순환
            }
        }
        string prevSongPath = "./songs/" + songList[currentSongIndex]; // 이전 곡 경로 생성
        playSong(prevSongPath); // 이전 곡 재생
    }



    void shufflePlaylist() // 재생 목록을 셔플하는 함수
    {
        isShuffle = !isShuffle; // 셔플 모드 토글
        if (isShuffle) // 활성화된 경우
        {
            shuffledList = playlist; // 기존 재생 목록 복사
            random_device rd;
            mt19937 g(rd());
            shuffle(shuffledList.begin(), shuffledList.end(), g); // 리스트 섞기
            cout << "[Shuffle mode enabled]" << endl; // 알림 출력
        }
        else // 비활성화된 경우
        {
            cout << "[Shuffle mode disabled]" << endl; // 알림 출력
        }
    }

    void toggleRepeat() // 반복 재생 모드를 토글하는 함수
    {
        isRepeat = !isRepeat; // 반복 모드 토글
        if (isRepeat) // 활성화된 경우
        {
            cout << "[Repeat mode enabled]" << endl; // 알림 출력
        }
        else // 비활성화된 경우
        {
            cout << "[Repeat mode disabled]" << endl; // 알림 출력
        }
    }

public:
    run() // 프로그램 실행 클래스의 생성자
    {
        while (true) // 프로그램 루프
        {
            border(); // 초기 화면 출력
            input = getchar(); // 사용자 입력 받기
            if (input == ' ') // 프로그램 종료
            {
                clearScreen; // 화면 초기화
                exit(0); // 프로그램 종료
            }
            else if (input == 'L' || input == 'l') // 노래 목록 보기
            {
                listSongs(); // 노래 목록 함수 호출
            }
        }
    }
};

int main(void)
{
    run MusicPlayer; // MusicPlayer 객체 생성 및 실행
    return 0; // 프로그램 정상 종료
}