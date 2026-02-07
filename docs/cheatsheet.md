# 컴파일러 & 빌드 시스템 핵심 정리 (암기용)

## 1. 셸 스크립트 문법

### 변수
```bash
name="value"          # 선언 (= 양쪽 공백 없음!)
echo "$name"          # 사용 (큰따옴표로 감싸기)
readonly PI=3.14      # 상수
```

### 특수 변수
| 변수 | 의미 |
|------|------|
| `$0` | 스크립트 이름 |
| `$1`, `$2`... | 첫째, 둘째... 인자 |
| `$#` | 인자 개수 |
| `$@` | 모든 인자 (개별) |
| `$*` | 모든 인자 (하나로) |
| `$?` | 마지막 명령 종료 코드 |
| `$$` | 현재 프로세스 ID |

### 조건문
```bash
if [ 조건 ]; then
    명령
elif [ 조건 ]; then
    명령
else
    명령
fi
```

### 비교 연산자
| 숫자 | 의미 | 문자열 | 의미 |
|------|------|--------|------|
| `-eq` | 같다 | `=` | 같다 |
| `-ne` | 다르다 | `!=` | 다르다 |
| `-lt` | 작다 (<) | `-z` | 비어있다 |
| `-le` | 작거나 같다 | `-n` | 비어있지 않다 |
| `-gt` | 크다 (>) | | |
| `-ge` | 크거나 같다 | | |

### 파일 테스트
| 연산자 | 의미 |
|--------|------|
| `-e` | 존재하는가 |
| `-f` | 일반 파일인가 |
| `-d` | 디렉토리인가 |
| `-r` | 읽기 가능한가 |
| `-w` | 쓰기 가능한가 |
| `-x` | 실행 가능한가 |

### 반복문
```bash
# for 루프
for i in 1 2 3; do
    echo "$i"
done

# while 루프
while [ 조건 ]; do
    명령
done
```

### 함수
```bash
function_name() {
    local var="지역변수"    # local 키워드!
    echo "$1"               # 첫 번째 인자
    return 0                # 반환값 (0=성공)
}
result=$(function_name "arg")  # 호출 및 출력 캡처
```

### 안전한 스크립트 시작
```bash
#!/bin/bash
set -e    # 오류 시 즉시 종료
set -u    # 미정의 변수 사용 시 오류
set -o pipefail  # 파이프라인 오류 전파
```

---

## 2. CMake 핵심

### 기본 구조
```cmake
cmake_minimum_required(VERSION 3.16)  # 최소 버전
project(프로젝트명)                    # 프로젝트 선언
add_executable(실행파일 소스.cpp)      # 실행 파일 생성
```

### 라이브러리
```cmake
add_library(라이브러리명 STATIC 소스.cpp)  # 정적 라이브러리
target_link_libraries(타겟 PRIVATE 라이브러리)  # 링크
```

### 가시성 키워드
| 키워드 | 의미 |
|--------|------|
| `PRIVATE` | 이 타겟만 사용 |
| `PUBLIC` | 이 타겟 + 이걸 링크하는 타겟도 사용 |
| `INTERFACE` | 이걸 링크하는 타겟만 사용 |

### 메시지 출력
```cmake
message(STATUS "정보")      # 일반 정보 (-- 접두사)
message(WARNING "경고")     # 경고 (계속 진행)
message(FATAL_ERROR "오류") # 오류 (빌드 중단)
```

### 빌드 명령
```bash
mkdir build && cd build
cmake ..                    # 빌드 시스템 생성
cmake --build .             # 컴파일
cmake --build . -j4         # 병렬 컴파일 (4코어)
```

---

## 3. 컴파일러 핵심 개념

### 컴파일 단계
```
소스코드 → [렉서] → 토큰 → [파서] → AST → [분석/생성] → 실행파일
           문자→단어    단어→문장구조
```

### 토큰 (Token)
- **정의**: 의미 있는 최소 단위 (단어)
- **구성**: 타입(TokenType) + 값(value) + 위치(position)

```cpp
enum class TokenType {
    INT,        // 123
    ID,         // 변수명
    PLUS,       // +
    LPAREN,     // (
    IF,         // 키워드 if
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string value;
    int line, column;
};
```

### 렉서 (Lexer) 핵심 패턴
```cpp
class Lexer {
    std::string input;
    size_t pos = 0;

    char peek() { return input[pos]; }           // 현재 문자 확인
    char advance() { return input[pos++]; }      // 문자 소비
    bool isAtEnd() { return pos >= input.size(); }

    Token scanToken() {
        skipWhitespace();
        if (isdigit(peek())) return scanNumber();
        if (isalpha(peek())) return scanIdentifier();
        // ...
    }
};
```

### 파서 (Parser) 핵심 패턴
```cpp
class Parser {
    std::vector<Token> tokens;
    size_t current = 0;

    Token peek() { return tokens[current]; }     // 현재 토큰 확인
    Token advance() { return tokens[current++]; } // 토큰 소비

    bool check(TokenType t) {                    // 타입 확인
        return peek().type == t;
    }

    bool match(TokenType t) {                    // 확인 + 소비
        if (check(t)) { advance(); return true; }
        return false;
    }

    Token expect(TokenType t) {                  // 반드시 있어야 함
        if (!check(t)) throw error("expected ...");
        return advance();
    }
};
```

### 재귀 하강 파싱
- **핵심**: 각 문법 규칙 = 하나의 함수
- **규칙**: `A → B C` 이면 A 함수에서 B, C 함수 순서대로 호출

```
// 문법
expr   → term (('+' | '-') term)*
term   → factor (('*' | '/') factor)*
factor → NUMBER | '(' expr ')'
```

```cpp
Exp* parseExpr() {
    Exp* left = parseTerm();
    while (match(PLUS) || match(MINUS)) {
        Op op = previousOp();
        Exp* right = parseTerm();
        left = new BinOpExp(left, op, right);
    }
    return left;
}
```

### 연산자 우선순위
| 우선순위 | 연산자 | 결합 방향 |
|----------|--------|-----------|
| 낮음 | `+`, `-` | 좌→우 |
| 높음 | `*`, `/` | 좌→우 |
| 최고 | 괄호, 리터럴 | - |

**규칙**: 낮은 우선순위가 파싱 트리 상단에 위치

---

## 4. AST (추상 구문 트리)

### 기본 구조
```cpp
enum class ExpKind { INT, VAR, BINOP, CALL };

struct Exp {
    ExpKind kind;
    Position pos;
    virtual ~Exp() = default;
};

using ExpPtr = std::unique_ptr<Exp>;
```

### 노드 타입
```cpp
struct IntExp : Exp {           // 정수 리터럴
    int value;
};

struct VarExp : Exp {           // 변수 참조
    std::string name;
};

struct BinOpExp : Exp {         // 이항 연산
    ExpPtr left;
    Op op;
    ExpPtr right;
};

struct CallExp : Exp {          // 함수 호출
    std::string func;
    std::vector<ExpPtr> args;
};
```

---

## 5. C++ 스마트 포인터

### unique_ptr
```cpp
// 생성
auto ptr = std::make_unique<Type>(args);

// 소유권 이전 (복사 불가!)
auto ptr2 = std::move(ptr);  // ptr은 이제 nullptr

// 컨테이너에 저장
vec.push_back(std::move(ptr));
```

### 핵심 규칙
1. `std::make_unique` 사용
2. 복사 불가, 이동만 가능
3. 소유권 이전 시 `std::move()` 필수
4. 자동 메모리 해제 (delete 불필요)

---

## 6. 암기 체크리스트

### 셸 스크립트
- [ ] `$1`, `$@`, `$#`, `$?` 의미
- [ ] `-eq`, `-lt`, `-gt` 비교 연산자
- [ ] `-e`, `-f`, `-d` 파일 테스트
- [ ] `set -euo pipefail` 의미

### CMake
- [ ] `cmake_minimum_required`, `project`, `add_executable`
- [ ] `PRIVATE`, `PUBLIC`, `INTERFACE` 차이
- [ ] `STATUS`, `WARNING`, `FATAL_ERROR` 차이

### 컴파일러
- [ ] 렉서 vs 파서 역할 구분
- [ ] `peek()`, `advance()`, `match()`, `expect()` 동작
- [ ] 재귀 하강: 문법 규칙 → 함수 매핑
- [ ] 연산자 우선순위: 낮은 게 위로

### C++
- [ ] `unique_ptr` 생성 및 이동
- [ ] `std::move()` 사용 시점
