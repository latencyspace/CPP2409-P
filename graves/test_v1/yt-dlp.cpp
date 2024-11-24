#include <iostream>
#include <cstdlib> // system() 함수를 사용하기 위한 헤더

using namespace std;

int main()
{
    string url;
    cout << "듣고 싶은 음악의 유튜브 URL을 입력하세요: ";
    getline(cin, url);

    // yt-dlp 명령어를 호출
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

    return 0;
}
