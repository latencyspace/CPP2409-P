## Commit Convention

커밋 요약은 "(type): (content)" 형식으로 작성합니다.

type은 영어 소문자로 작성하며, content는 한국어로 명확하게 작성합니다. 요약이 너무 길어지면 세부적인 내용은 커밋 설명에 풀어적고 요약은 간결하게 수정합니다.

| type     | 설명                                                                      | 예시                                                               |
| -------- | ------------------------------------------------------------------------- | ------------------------------------------------------------------ |
| feat     | 새로운 기능 추가                                                          | feat: 로그인 페이지 구현                                           |
| fix      | 버그 수정                                                                 | fix: 320px보다 작은 기기에서 "다음" 버튼이 잘리는 문제 수정        |
| docs     | 문서 수정                                                                 | docs: README.md에 커밋 컨벤션 추가                                 |
| style    | 코드 포맷팅, 세미콜론 누락, 코드 변경이 없는 경우                         | style: 모든 따옴표를 쌍따옴표로 수정                               |
| refactor | 코드 리팩토링                                                             | refactor: axios request를 axios instance를 이용해서 중복 코드 정리 |
| test     | 테스트 코드 구현                                                          | test: postLogin, postRegister 함수의 테스트 케이스 추가            |
| chore    | 패키지 매니저 수정, 빌드 CI/CD 설정 수정, 깃허브 설정 수정 등 잡다한 일들 | chore: package.json에 redux 패키지 추가                            |

## CLI에서 Youtube 영상 파일 다운로드 받는 방법

1. python3 -m venv path/to/venv
2. source path/to/venv/bin/activate
3. yt-dlp 파일이 있는 폴더로 이동
4. 터미널에서 ./yt-dlp 실행
5. URL 입력
