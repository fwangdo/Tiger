# 빌드 시스템과 셸 스크립트 완벽 가이드

C++ 프로젝트를 CMake와 셸 스크립트로 자동화하는 방법을 배웁니다.

---

## 목차

1. [Part 1: 셸 스크립트 기초](#part-1-셸-스크립트-기초)
2. [Part 2: CMake 기초](#part-2-cmake-기초)
3. [Part 3: CMake 심화](#part-3-cmake-심화)
4. [Part 4: 빌드 자동화 스크립트](#part-4-빌드-자동화-스크립트)

---

## Part 1: 셸 스크립트 기초

### 1.1 셸 스크립트란?

셸 스크립트는 터미널에서 실행할 명령어들을 파일에 모아놓은 것입니다.
매번 같은 명령어를 반복 입력하는 대신, 스크립트 파일 하나로 자동화할 수 있습니다.

```bash
#!/bin/bash
# 이것은 주석입니다
echo "Hello, World!"
```

**첫 번째 줄 `#!/bin/bash` 설명:**
- `#!`는 "shebang"이라고 부릅니다
- 이 스크립트를 어떤 프로그램으로 실행할지 지정합니다
- `/bin/bash`는 Bash 셸의 위치입니다
- 즉, "이 파일을 bash로 실행해주세요"라는 의미입니다

### 1.2 스크립트 만들고 실행하기

```bash
# 1. 스크립트 파일 만들기
cat > myscript.sh << 'EOF'
#!/bin/bash
echo "스크립트가 실행되었습니다!"
EOF

# 2. 실행 권한 부여하기 (중요!)
chmod +x myscript.sh

# 3. 실행하기
./myscript.sh
```

**`chmod +x`가 필요한 이유:**
- 리눅스/맥에서 파일은 기본적으로 실행 권한이 없습니다
- `+x`는 "실행(eXecute) 권한을 추가(+)하라"는 의미입니다
- 이 명령 없이 `./myscript.sh`를 실행하면 "Permission denied" 오류가 납니다

### 1.3 변수

```bash
#!/bin/bash

# 변수 할당 (= 주위에 공백 없이!)
NAME="Tiger"
VERSION=1.0
BUILD_DIR="build"

# 잘못된 예 (오류 발생):
# NAME = "Tiger"    # 공백이 있으면 안 됨!

# 변수 사용하기 ($변수명 또는 ${변수명})
echo "Building $NAME version $VERSION"
echo "Output directory: ${BUILD_DIR}"

# ${} 를 쓰는 이유:
# echo "$NAMEcompiler"  # NAME 다음에 바로 문자가 오면 NAMEcompiler라는 변수를 찾음
# echo "${NAME}compiler" # 이렇게 해야 NAME 변수 + "compiler" 문자열이 됨
```

**명령어 결과를 변수에 저장하기 (Command Substitution):**

```bash
# $(명령어) 형식으로 명령어 실행 결과를 변수에 저장
CURRENT_DIR=$(pwd)           # pwd 명령의 결과 저장
TODAY=$(date +%Y-%m-%d)      # 오늘 날짜 저장
FILE_COUNT=$(ls -1 | wc -l)  # 파일 개수 저장

echo "현재 디렉토리: $CURRENT_DIR"
echo "오늘 날짜: $TODAY"
echo "파일 개수: $FILE_COUNT"
```

**기본값 설정하기:**

```bash
# 변수가 설정되지 않았으면 기본값 사용
BUILD_TYPE=${BUILD_TYPE:-Release}

# 설명:
# ${변수:-기본값} 형식
# BUILD_TYPE이 비어있거나 설정 안 됐으면 "Release" 사용
# 이미 값이 있으면 그 값 유지

# 사용 예:
# $ ./build.sh              # BUILD_TYPE="Release" (기본값)
# $ BUILD_TYPE=Debug ./build.sh  # BUILD_TYPE="Debug" (지정한 값)
```

### 1.4 특수 변수

스크립트에서 자동으로 사용할 수 있는 특수 변수들:

| 변수 | 의미 | 예시 |
|------|------|------|
| `$0` | 스크립트 파일 이름 | `./build.sh` |
| `$1`, `$2`, ... | 첫 번째, 두 번째, ... 인자 | `./build.sh Debug` → `$1`="Debug" |
| `$#` | 인자 개수 | 인자가 3개면 `$#`=3 |
| `$@` | 모든 인자 (개별 단어로) | "arg1" "arg2" "arg3" |
| `$?` | 직전 명령의 종료 코드 | 성공=0, 실패=1 이상 |
| `$$` | 현재 스크립트의 프로세스 ID | 12345 |

```bash
#!/bin/bash

echo "스크립트 이름: $0"
echo "첫 번째 인자: $1"
echo "두 번째 인자: $2"
echo "인자 개수: $#"
echo "모든 인자: $@"

# 실행 예: ./test.sh hello world
# 출력:
#   스크립트 이름: ./test.sh
#   첫 번째 인자: hello
#   두 번째 인자: world
#   인자 개수: 2
#   모든 인자: hello world
```

### 1.5 조건문

#### 기본 if-else 구조

```bash
#!/bin/bash

if [ 조건 ]; then
    # 조건이 참일 때 실행
    echo "조건이 참입니다"
elif [ 다른조건 ]; then
    # 첫 번째 조건은 거짓이고, 다른조건이 참일 때
    echo "다른 조건이 참입니다"
else
    # 모든 조건이 거짓일 때
    echo "모든 조건이 거짓입니다"
fi  # if를 거꾸로 쓴 fi로 끝냄
```

**주의:** `[`와 `]` 안쪽에 반드시 공백이 있어야 합니다!
```bash
if [ -f "$FILE" ]; then   # 올바름
if [-f "$FILE"]; then     # 오류! 공백 필요
```

#### 문자열 비교

```bash
#!/bin/bash

STRING="hello"

# 문자열이 같은지
if [ "$STRING" = "hello" ]; then
    echo "문자열이 hello입니다"
fi

# 문자열이 다른지
if [ "$STRING" != "world" ]; then
    echo "문자열이 world가 아닙니다"
fi

# 문자열이 비어있는지 (-z: zero length)
if [ -z "$STRING" ]; then
    echo "문자열이 비어있습니다"
fi

# 문자열이 비어있지 않은지 (-n: non-zero length)
if [ -n "$STRING" ]; then
    echo "문자열이 비어있지 않습니다"
fi
```

**왜 변수를 따옴표로 감싸나요?**
```bash
FILE="my file.txt"  # 공백이 포함된 파일명

# 따옴표 없이:
if [ -f $FILE ]; then    # 오류! "my"와 "file.txt"로 분리됨

# 따옴표 있으면:
if [ -f "$FILE" ]; then  # 정상! "my file.txt" 전체가 하나로 처리
```

#### 숫자 비교

숫자 비교는 `=` 대신 특별한 연산자를 사용합니다:

| 연산자 | 의미 | 영어 원형 |
|--------|------|-----------|
| `-eq` | 같다 (==) | **eq**ual |
| `-ne` | 다르다 (!=) | **n**ot **e**qual |
| `-lt` | 작다 (<) | **l**ess **t**han |
| `-le` | 작거나 같다 (<=) | **l**ess or **e**qual |
| `-gt` | 크다 (>) | **g**reater **t**han |
| `-ge` | 크거나 같다 (>=) | **g**reater or **e**qual |

```bash
#!/bin/bash

NUM=10

if [ "$NUM" -eq 10 ]; then
    echo "NUM은 10입니다"
fi

if [ "$NUM" -gt 5 ]; then
    echo "NUM은 5보다 큽니다"
fi

if [ "$NUM" -le 20 ]; then
    echo "NUM은 20 이하입니다"
fi
```

#### 파일/디렉토리 검사

| 연산자 | 의미 | 영어 원형 |
|--------|------|-----------|
| `-e` | 존재하는가 | **e**xists |
| `-f` | 일반 파일인가 | **f**ile |
| `-d` | 디렉토리인가 | **d**irectory |
| `-r` | 읽기 가능한가 | **r**eadable |
| `-w` | 쓰기 가능한가 | **w**ritable |
| `-x` | 실행 가능한가 | e**x**ecutable |
| `-s` | 파일 크기가 0보다 큰가 | **s**ize > 0 |

```bash
#!/bin/bash

FILE="config.txt"
DIR="build"

# 파일이 존재하는가?
if [ -e "$FILE" ]; then
    echo "파일이 존재합니다"
fi

# 일반 파일인가? (디렉토리가 아닌)
if [ -f "$FILE" ]; then
    echo "일반 파일입니다"
fi

# 디렉토리인가?
if [ -d "$DIR" ]; then
    echo "디렉토리입니다"
fi

# 파일이 존재하지 않는가? (! = NOT)
if [ ! -e "$FILE" ]; then
    echo "파일이 존재하지 않습니다"
fi
```

#### 논리 연산자

```bash
#!/bin/bash

# AND: 둘 다 참이어야 참
if [ -f "$FILE" ] && [ -r "$FILE" ]; then
    echo "파일이 존재하고 읽기 가능합니다"
fi

# OR: 둘 중 하나만 참이면 참
if [ -f "$FILE" ] || [ -d "$FILE" ]; then
    echo "파일이거나 디렉토리입니다"
fi

# NOT: 조건 반전
if [ ! -e "$FILE" ]; then
    echo "존재하지 않습니다"
fi
```

### 1.6 반복문

#### for 반복문

```bash
#!/bin/bash

# 리스트 순회
for fruit in apple banana cherry; do
    echo "과일: $fruit"
done
# 출력:
#   과일: apple
#   과일: banana
#   과일: cherry

# 파일 순회 (glob 패턴 사용)
for file in *.cpp; do
    echo "C++ 파일: $file"
done

# C 스타일 for 문 (숫자 반복)
for ((i = 0; i < 5; i++)); do
    echo "인덱스: $i"
done
# 출력: 0, 1, 2, 3, 4

# 인자 순회
for arg in "$@"; do
    echo "인자: $arg"
done
```

#### while 반복문

```bash
#!/bin/bash

# 조건이 참인 동안 반복
count=0
while [ $count -lt 5 ]; do
    echo "카운트: $count"
    ((count++))  # count를 1 증가
done

# 파일을 한 줄씩 읽기
while IFS= read -r line; do
    echo "줄: $line"
done < input.txt

# 무한 루프 + break
while true; do
    echo "실행 중..."
    if [ 어떤조건 ]; then
        break  # 루프 탈출
    fi
    sleep 1  # 1초 대기
done
```

### 1.7 함수

```bash
#!/bin/bash

# 함수 정의
greet() {
    echo "안녕하세요, $1님!"  # $1 = 첫 번째 인자
}

# 함수 호출
greet "홍길동"
# 출력: 안녕하세요, 홍길동님!

# 값을 반환하는 함수 (echo로 출력)
add_numbers() {
    local result=$(( $1 + $2 ))  # local: 지역 변수
    echo $result  # 결과를 "반환" (실제로는 출력)
}

# 함수 결과 받기
sum=$(add_numbers 5 3)
echo "합계: $sum"  # 합계: 8

# 성공/실패를 반환하는 함수 (return 사용)
file_exists() {
    if [ -f "$1" ]; then
        return 0  # 성공 (유닉스에서 0 = 성공)
    else
        return 1  # 실패
    fi
}

# 함수 결과 확인
if file_exists "config.txt"; then
    echo "파일이 있습니다"
else
    echo "파일이 없습니다"
fi
```

**`local` 키워드:**
- 함수 안에서만 사용되는 지역 변수를 만듭니다
- `local` 없이 변수를 만들면 전역 변수가 되어 함수 밖에서도 값이 유지됩니다

### 1.8 오류 처리

```bash
#!/bin/bash

# 오류 발생 시 즉시 중단
set -e

# 정의되지 않은 변수 사용 시 오류
set -u

# 파이프라인에서 오류 발생 시 전체 실패
set -o pipefail

# 위 세 개를 한 번에 (권장)
set -euo pipefail
```

**각 옵션 설명:**

`set -e` (errexit):
```bash
set -e
mkdir /invalid/path  # 실패하면 스크립트 즉시 종료
echo "이 줄은 실행되지 않음"
```

`set -u` (nounset):
```bash
set -u
echo $UNDEFINED_VAR  # 오류! 정의되지 않은 변수
```

`set -o pipefail`:
```bash
set -o pipefail
cat nonexistent.txt | grep "pattern"  # cat이 실패하면 전체 실패
# pipefail 없으면 grep의 결과(성공)만 반환
```

**오류 처리 함수:**

```bash
#!/bin/bash
set -euo pipefail

# 오류 메시지 출력하고 종료하는 함수
die() {
    echo "오류: $1" >&2  # >&2 = 표준 오류로 출력
    exit "${2:-1}"       # 두 번째 인자가 없으면 종료 코드 1
}

# 사용 예
[ -f "config.txt" ] || die "config.txt 파일이 필요합니다"
```

### 1.9 인자 파싱 (getopts)

명령줄 옵션을 처리하는 표준 방법:

```bash
#!/bin/bash

# 기본값 설정
DEBUG=false
OUTPUT_FILE="output.txt"
VERBOSE=0

# 사용법 출력 함수
usage() {
    cat << EOF
사용법: $0 [옵션] <입력파일>

옵션:
    -d          디버그 모드
    -o 파일     출력 파일 지정 (기본: output.txt)
    -v          자세한 출력 (여러 번 사용 가능)
    -h          도움말 출력
EOF
    exit 0
}

# getopts로 옵션 파싱
# "do:vh" 의미:
#   d  = -d 옵션 (인자 없음)
#   o: = -o 옵션 (: 가 있으면 인자 필요)
#   v  = -v 옵션 (인자 없음)
#   h  = -h 옵션 (인자 없음)
while getopts "do:vh" opt; do
    case $opt in
        d) DEBUG=true ;;
        o) OUTPUT_FILE="$OPTARG" ;;  # OPTARG = 옵션의 인자값
        v) ((VERBOSE++)) ;;          # -v -v 하면 2가 됨
        h) usage ;;
        ?) usage ;;  # 잘못된 옵션
    esac
done

# 처리된 옵션들을 건너뛰기
shift $((OPTIND - 1))

# 나머지 인자 (옵션이 아닌 것들)
INPUT_FILE="${1:-}"
if [ -z "$INPUT_FILE" ]; then
    echo "오류: 입력 파일을 지정하세요" >&2
    usage
fi

echo "DEBUG: $DEBUG"
echo "OUTPUT: $OUTPUT_FILE"
echo "VERBOSE: $VERBOSE"
echo "INPUT: $INPUT_FILE"
```

**실행 예:**
```bash
./script.sh -d -o result.txt -v -v input.txt
# DEBUG: true
# OUTPUT: result.txt
# VERBOSE: 2
# INPUT: input.txt
```

---

## Part 2: CMake 기초

### 2.1 CMake란?

CMake는 **빌드 시스템 생성기**입니다.

**빌드 시스템이란?**
- 소스 코드를 실행 파일로 만드는 과정을 자동화하는 도구
- 예: Make, Ninja, Visual Studio

**CMake는 왜 필요한가?**
- 플랫폼마다 빌드 도구가 다릅니다 (Linux=Make, Windows=Visual Studio)
- CMake로 작성하면 모든 플랫폼에서 동작하는 빌드 설정을 만들 수 있습니다

```
┌──────────────────┐     ┌─────────────┐     ┌──────────────┐
│ CMakeLists.txt   │────▶│   CMake     │────▶│  Makefile    │
│ (설정 파일)       │     │ (변환기)     │     │  (빌드 파일)  │
└──────────────────┘     └─────────────┘     └──────────────┘
                                                    │
                                                    ▼
                                             ┌──────────────┐
                                             │  실행 파일    │
                                             └──────────────┘
```

### Summary of whole procedure. 

The procedure that c++ makes a binary file as follows: 
1. Generate object files(.o) from source files(.cpp)
2. Link object files(.o) and library files, then the result will be the binary file which is our goal. 

add_executable means "generate binary file by given source codes."
add_ means "generate binary file by given source codes."

### 2.2 기본 CMakeLists.txt

```cmake
# CMake 최소 버전 요구
# 이 버전 이상의 CMake가 필요하다고 선언
cmake_minimum_required(VERSION 3.16)

# 프로젝트 정보 선언
# NAME: 프로젝트 이름
# VERSION: 버전 번호
# LANGUAGES: 사용하는 프로그래밍 언어
project(MyCompiler VERSION 1.0 LANGUAGES CXX)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)        # C++17 사용
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 필수 (지원 안 되면 오류)

# 실행 파일 만들기
# add_executable(실행파일이름 소스파일들...)
add_executable(mycompiler
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
)

# 헤더 파일 디렉토리 추가
# PRIVATE: 이 타겟에서만 사용 (다른 타겟에 전파 안 함)
target_include_directories(mycompiler PRIVATE src)
```

### 2.3 CMake로 빌드하기

```bash
# 1. 빌드 디렉토리 만들기 (소스와 빌드 분리)
mkdir build
cd build

# 2. CMake 설정 (Configure)
# ".." = 상위 디렉토리의 CMakeLists.txt를 읽음
cmake ..

# 3. 빌드 (Build)
cmake --build .

# 또는 간단히:
make          # Linux/Mac
ninja         # Ninja 사용 시
```

**왜 별도의 build 디렉토리를 만드나요?**
- 소스 파일과 빌드 결과물을 분리합니다
- `rm -rf build`로 빌드 결과물만 깔끔하게 삭제할 수 있습니다
- 여러 빌드 설정(Debug, Release)을 각각 다른 디렉토리에 만들 수 있습니다

### 2.4 CMake 변수

```cmake
# 변수 설정
set(MY_VAR "값")
set(MY_LIST "item1" "item2" "item3")

# 변수 사용 (${변수명})
message(STATUS "내 변수: ${MY_VAR}")
```

**message() 함수 설명:**

```cmake
# message(모드 "메시지")
# 모드 종류:
message(STATUS "상태 메시지")     # -- 상태 메시지 (일반 정보)
message(WARNING "경고 메시지")   # CMake Warning: 경고 메시지
message(FATAL_ERROR "오류!")    # 오류 출력 후 CMake 중단
message("그냥 메시지")          # 모드 없이도 가능
```

**STATUS의 의미:**
- `STATUS`는 일반적인 정보성 메시지를 의미합니다
- 출력 시 앞에 `--`가 붙습니다
- 예: `message(STATUS "빌드 타입: Release")` → `-- 빌드 타입: Release`

**캐시 변수 (사용자 설정 가능):**

```cmake
# 캐시 변수: CMake GUI나 명령줄에서 변경 가능
# set(변수명 기본값 CACHE 타입 "설명")
set(MY_OPTION "default" CACHE STRING "옵션 설명")

# 타입 종류:
# STRING - 문자열
# BOOL   - ON/OFF
# PATH   - 디렉토리 경로
# FILEPATH - 파일 경로
```

**option() - 불리언 옵션:**

```cmake
# option(변수명 "설명" 기본값)
option(ENABLE_TESTS "테스트 빌드 여부" ON)

# 사용:
if(ENABLE_TESTS)
    # 테스트 관련 설정
endif()
```

명령줄에서 변경:
```bash
cmake -DENABLE_TESTS=OFF ..
```

### 2.5 중요한 내장 변수

| 변수 | 설명 | 예시 값 |
|------|------|---------|
| `CMAKE_SOURCE_DIR` | 최상위 소스 디렉토리 | `/home/user/project` |
| `CMAKE_BINARY_DIR` | 최상위 빌드 디렉토리 | `/home/user/project/build` |
| `CMAKE_CURRENT_SOURCE_DIR` | 현재 CMakeLists.txt 위치 | `/home/user/project/src` |
| `CMAKE_BUILD_TYPE` | 빌드 타입 | `Debug`, `Release` |
| `CMAKE_CXX_COMPILER` | C++ 컴파일러 경로 | `/usr/bin/g++` |
| `PROJECT_NAME` | 프로젝트 이름 | `MyCompiler` |
| `PROJECT_VERSION` | 프로젝트 버전 | `1.0` |

### 2.6 빌드 타입 (Build Type)

```cmake
# 빌드 타입이 지정되지 않았으면 Release로 설정
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "빌드 타입" FORCE)
endif()
```

**빌드 타입 종류:**

| 타입 | 설명 | 컴파일러 플래그 |
|------|------|----------------|
| `Debug` | 디버깅용 | `-g` (디버그 심볼), 최적화 없음 |
| `Release` | 배포용 | `-O3` (최대 최적화), `-DNDEBUG` |
| `RelWithDebInfo` | 배포 + 디버그 정보 | `-O2 -g` |
| `MinSizeRel` | 크기 최소화 | `-Os` |

**명령줄에서 빌드 타입 지정:**
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake -DCMAKE_BUILD_TYPE=Release ..
```

### 2.7 라이브러리 만들기

```cmake
# 정적 라이브러리 (Static Library)
# 정적 링킹 파일을 만듦(.a)
# 컴파일 시 실행 파일에 포함됨
add_library(mylib STATIC
    src/lexer.cpp
    src/parser.cpp
)

# 공유 라이브러리 (Shared Library)
# 실행 시 동적으로 로드됨 (.so, .dll)
add_library(mylib SHARED
    src/lexer.cpp
    src/parser.cpp
)
```

**라이브러리 연결:**

```cmake
# target_link_libraries(타겟 가시성 라이브러리...)
target_link_libraries(mycompiler PRIVATE mylib)
```

**가시성(Visibility) 키워드:**

| 키워드 | 의미 |
|--------|------|
| `PRIVATE` | 이 타겟에서만 사용 |
| `PUBLIC` | 이 타겟과 이 타겟을 링크하는 다른 타겟에서도 사용 |
| `INTERFACE` | 이 타겟에서는 사용 안 하고, 링크하는 다른 타겟에서만 사용 |

```cmake
# 예시:
# mylib이 내부적으로 zlib을 사용하고,
# mylib의 헤더에 zlib 타입이 노출되어 있다면:
target_link_libraries(mylib PUBLIC zlib)

# mylib이 내부적으로만 zlib을 사용하고,
# 헤더에는 노출되지 않는다면:
target_link_libraries(mylib PRIVATE zlib)
```

### 2.8 컴파일 옵션 추가

```cmake
# 컴파일러 경고 옵션 추가
target_compile_options(mycompiler PRIVATE
    -Wall      # 일반적인 경고 활성화
    -Wextra    # 추가 경고 활성화
    -Wpedantic # 엄격한 표준 준수 경고
)

# 조건부 옵션 (GCC나 Clang일 때만)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mycompiler PRIVATE -Werror)
endif()
```

**전처리기 매크로 정의:**

```cmake
# target_compile_definitions(타겟 가시성 정의...)
target_compile_definitions(mycompiler PRIVATE
    DEBUG_MODE          # #define DEBUG_MODE
    VERSION="1.0"       # #define VERSION "1.0"
)
```

### 2.9 하위 디렉토리

큰 프로젝트는 여러 디렉토리로 나눕니다:

```
project/
├── CMakeLists.txt       # 루트
├── src/
│   ├── CMakeLists.txt   # src 디렉토리용
│   └── main.cpp
└── lib/
    ├── CMakeLists.txt   # lib 디렉토리용
    ├── lexer.cpp
    └── parser.cpp
```

**루트 CMakeLists.txt:**
```cmake
cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX)

# 하위 디렉토리 추가
add_subdirectory(lib)  # lib/CMakeLists.txt 실행
add_subdirectory(src)  # src/CMakeLists.txt 실행
```

**lib/CMakeLists.txt:**
```cmake
add_library(mylib STATIC lexer.cpp parser.cpp)
target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
```

**src/CMakeLists.txt:**
```cmake
add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE mylib)
```

---

## Part 3: CMake 심화

### 3.1 조건문

```cmake
# if-elseif-else-endif
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "디버그 빌드입니다")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "릴리즈 빌드입니다")
else()
    message(STATUS "기타 빌드 타입입니다")
endif()

# STREQUAL: 문자열 비교
# EQUAL: 숫자 비교
# MATCHES: 정규식 매칭

# 변수가 정의되어 있는지 확인
if(DEFINED MY_VAR)
    message(STATUS "MY_VAR가 정의되어 있습니다")
endif()

# 불리언 옵션 확인
if(ENABLE_TESTS)
    add_subdirectory(tests)
endif()

# 파일/디렉토리 존재 확인
if(EXISTS "${CMAKE_SOURCE_DIR}/optional.cpp")
    list(APPEND SOURCES optional.cpp)
endif()
```

### 3.2 Generator Expressions (생성기 표현식)

빌드 시점에 평가되는 조건식입니다. 설정(Configure) 시점이 아닌 빌드 시점에 값이 결정됩니다.

```cmake
# $<조건:참일때값>
# $<CONFIG:Debug> = Debug 빌드일 때 1, 아니면 0

# Debug 빌드에서만 DEBUG 매크로 정의
target_compile_definitions(myapp PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)
# Debug일 때: -DDEBUG
# Release일 때: (아무것도 안 함)

# Release 빌드에서만 -O3 최적화
target_compile_options(myapp PRIVATE
    $<$<CONFIG:Release>:-O3>
)

# 컴파일러에 따라 다른 옵션
target_compile_options(myapp PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-fno-rtti>
    $<$<CXX_COMPILER_ID:Clang>:-fno-rtti>
    $<$<CXX_COMPILER_ID:MSVC>:/GR->
)

# 플랫폼에 따라 다른 매크로
target_compile_definitions(myapp PRIVATE
    $<$<PLATFORM_ID:Windows>:WINDOWS>
    $<$<PLATFORM_ID:Linux>:LINUX>
    $<$<PLATFORM_ID:Darwin>:MACOS>
)
```

### 3.3 테스트 (CTest)

```cmake
# 테스트 활성화
enable_testing()

# 테스트 실행 파일 추가
add_executable(test_lexer tests/test_lexer.cpp)
target_link_libraries(test_lexer PRIVATE mylib)

# 테스트 등록
# add_test(NAME 테스트이름 COMMAND 실행할명령)
add_test(NAME LexerTest COMMAND test_lexer)

# 인자와 함께 테스트
add_test(NAME ParserTest
    COMMAND mycompiler --parse ${CMAKE_SOURCE_DIR}/tests/sample.txt
)

# 테스트 속성 설정
set_tests_properties(LexerTest PROPERTIES
    TIMEOUT 30        # 30초 타임아웃
    LABELS "unit"     # 라벨 지정
)
```

**테스트 실행:**
```bash
# 빌드 디렉토리에서
ctest                    # 모든 테스트 실행
ctest -V                 # 자세한 출력
ctest -R Lexer           # "Lexer"가 포함된 테스트만
ctest -L unit            # "unit" 라벨 테스트만
ctest --output-on-failure  # 실패한 테스트의 출력 표시
```

### 3.4 설치 (Install)

```cmake
# 실행 파일 설치
install(TARGETS mycompiler
    RUNTIME DESTINATION bin  # 실행 파일 → bin/
)

# 라이브러리 설치
install(TARGETS mylib
    ARCHIVE DESTINATION lib  # 정적 라이브러리 → lib/
    LIBRARY DESTINATION lib  # 공유 라이브러리 → lib/
)

# 헤더 파일 설치
install(FILES src/mylib.h
    DESTINATION include
)

# 디렉토리 전체 설치
install(DIRECTORY include/
    DESTINATION include
)
```

**설치 실행:**
```bash
# 기본 위치 (/usr/local)에 설치
cmake --install .

# 다른 위치에 설치
cmake -DCMAKE_INSTALL_PREFIX=/opt/myapp ..
cmake --build .
cmake --install .
```

---

## Part 4: 빌드 자동화 스크립트

### 4.1 기본 빌드 스크립트

```bash
#!/bin/bash
# build.sh - 기본 빌드 스크립트

set -euo pipefail  # 오류 발생 시 즉시 중단

# 설정
PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"
BUILD_TYPE="Release"

# 색상 (터미널 출력용)
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'  # No Color (색상 초기화)

# 로그 함수
log_info() {
    echo -e "${GREEN}[INFO]${NC} $*"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $*" >&2
}

# cmake가 설치되어 있는지 확인
check_cmake() {
    if ! command -v cmake &>/dev/null; then
        log_error "cmake가 설치되어 있지 않습니다"
        exit 1
    fi
}

# 설정 (Configure)
configure() {
    log_info "프로젝트 설정 중... (${BUILD_TYPE})"
    mkdir -p "${BUILD_DIR}"
    cmake -S "${PROJECT_ROOT}" \
          -B "${BUILD_DIR}" \
          -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"
}

# 빌드 (Build)
build() {
    log_info "빌드 중..."
    cmake --build "${BUILD_DIR}" --parallel
}

# 정리 (Clean)
clean() {
    log_info "빌드 디렉토리 삭제 중..."
    rm -rf "${BUILD_DIR}"
}

# 테스트 (Test)
test() {
    log_info "테스트 실행 중..."
    ctest --test-dir "${BUILD_DIR}" --output-on-failure
}

# 메인
main() {
    check_cmake
    configure
    build
    log_info "완료!"
}

main "$@"
```

### 4.2 옵션이 있는 빌드 스크립트

```bash
#!/bin/bash
# build.sh - 옵션을 지원하는 빌드 스크립트
#
# 사용법:
#   ./build.sh [옵션] [명령]
#
# 명령:
#   configure   프로젝트 설정
#   build       빌드 (기본)
#   test        테스트 실행
#   clean       빌드 디렉토리 삭제
#   all         설정 + 빌드 + 테스트
#
# 옵션:
#   -t, --type TYPE   빌드 타입 (Debug/Release)
#   -j, --jobs N      병렬 작업 수
#   -c, --clean       빌드 전 정리
#   -v, --verbose     자세한 출력
#   -h, --help        도움말

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# 기본값
BUILD_TYPE="Release"
PARALLEL_JOBS=""
CLEAN_FIRST=false
VERBOSE=false
COMMAND="build"

# 색상
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

log_info()  { echo -e "${GREEN}[INFO]${NC} $*"; }
log_warn()  { echo -e "${YELLOW}[WARN]${NC} $*"; }
log_error() { echo -e "${RED}[ERROR]${NC} $*" >&2; }

# 도움말
show_help() {
    grep '^#' "$0" | grep -v '#!/' | sed 's/^# \?//'
    exit 0
}

# CPU 코어 수 감지
detect_cores() {
    if command -v nproc &>/dev/null; then
        nproc
    elif command -v sysctl &>/dev/null; then
        sysctl -n hw.ncpu
    else
        echo 4
    fi
}

# 인자 파싱
parse_args() {
    while [[ $# -gt 0 ]]; do
        case "$1" in
            -t|--type)
                BUILD_TYPE="$2"
                shift 2
                ;;
            -j|--jobs)
                PARALLEL_JOBS="$2"
                shift 2
                ;;
            -c|--clean)
                CLEAN_FIRST=true
                shift
                ;;
            -v|--verbose)
                VERBOSE=true
                shift
                ;;
            -h|--help)
                show_help
                ;;
            -*)
                log_error "알 수 없는 옵션: $1"
                exit 1
                ;;
            *)
                COMMAND="$1"
                shift
                ;;
        esac
    done
}

# 빌드 디렉토리 설정
setup_dirs() {
    local type_lower
    type_lower=$(echo "${BUILD_TYPE}" | tr '[:upper:]' '[:lower:]')
    BUILD_DIR="${SCRIPT_DIR}/build/${type_lower}"

    PARALLEL_JOBS="${PARALLEL_JOBS:-$(detect_cores)}"
}

# 명령 실행 함수들
do_configure() {
    log_info "설정 중 (${BUILD_TYPE})..."
    mkdir -p "${BUILD_DIR}"

    local cmake_args=(
        -S "${SCRIPT_DIR}"
        -B "${BUILD_DIR}"
        -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    )

    cmake "${cmake_args[@]}"
}

do_build() {
    log_info "빌드 중 (${PARALLEL_JOBS}개 병렬)..."
    cmake --build "${BUILD_DIR}" --parallel "${PARALLEL_JOBS}"
}

do_test() {
    log_info "테스트 실행 중..."
    ctest --test-dir "${BUILD_DIR}" --output-on-failure
}

do_clean() {
    log_info "정리 중..."
    rm -rf "${BUILD_DIR}"
}

# 메인
main() {
    parse_args "$@"
    setup_dirs

    log_info "빌드 타입: ${BUILD_TYPE}"
    log_info "빌드 디렉토리: ${BUILD_DIR}"

    if [[ "${CLEAN_FIRST}" == true ]]; then
        do_clean
    fi

    case "${COMMAND}" in
        configure)  do_configure ;;
        build)      do_configure; do_build ;;
        test)       do_test ;;
        clean)      do_clean ;;
        all)        do_configure; do_build; do_test ;;
        *)          log_error "알 수 없는 명령: ${COMMAND}"; exit 1 ;;
    esac

    log_info "완료!"
}

main "$@"
```

### 4.3 사용 예시

```bash
# 기본 빌드 (Release)
./build.sh

# Debug 빌드
./build.sh --type Debug

# 정리 후 빌드
./build.sh --clean build

# 전체 (설정 + 빌드 + 테스트)
./build.sh --type Debug all

# 병렬 작업 수 지정
./build.sh -j 4 build
```

---

## 요약

### 셸 스크립트 핵심

| 개념 | 문법 | 예시 |
|------|------|------|
| 변수 할당 | `VAR="값"` | `NAME="tiger"` |
| 변수 사용 | `$VAR`, `${VAR}` | `echo "$NAME"` |
| 명령 치환 | `$(명령)` | `TODAY=$(date)` |
| 기본값 | `${VAR:-기본값}` | `${TYPE:-Release}` |
| 조건문 | `if [ ]; then fi` | `if [ -f "$F" ]; then` |
| 반복문 | `for in; do done` | `for f in *.cpp; do` |
| 함수 | `func() { }` | `log() { echo "$1"; }` |
| 오류 처리 | `set -euo pipefail` | 스크립트 시작 부분에 |

### CMake 핵심

| 개념 | 명령 | 설명 |
|------|------|------|
| 최소 버전 | `cmake_minimum_required(VERSION 3.16)` | 필요한 CMake 버전 |
| 프로젝트 | `project(Name LANGUAGES CXX)` | 프로젝트 선언 |
| 실행 파일 | `add_executable(name src...)` | 실행 파일 생성 |
| 라이브러리 | `add_library(name STATIC src...)` | 라이브러리 생성 |
| 링크 | `target_link_libraries(a PRIVATE b)` | 라이브러리 연결 |
| 포함 경로 | `target_include_directories(a PRIVATE dir)` | 헤더 경로 추가 |
| 메시지 | `message(STATUS "...")` | 정보 출력 |
| 옵션 | `option(VAR "설명" ON)` | 불리언 옵션 |

### 빌드 명령

```bash
# CMake 설정
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# 빌드
cmake --build build --parallel

# 테스트
ctest --test-dir build --output-on-failure

# 설치
cmake --install build
```
