# 컴파일러 구현 가이드: 렉서와 파서

프로그래밍 언어의 파서를 직접 구현하는 방법을 배웁니다.

---

## 목차

1. [Part 1: 컴파일러 개요](#part-1-컴파일러-개요)
2. [Part 2: 렉서 (Lexer)](#part-2-렉서-lexer)
3. [Part 3: 파서 (Parser)](#part-3-파서-parser)
4. [Part 4: AST (추상 구문 트리)](#part-4-ast-추상-구문-트리)

---

## Part 1: 컴파일러 개요

### 1.1 컴파일러란?

컴파일러는 프로그래밍 언어로 작성된 소스 코드를 다른 형태(기계어, 바이트코드 등)로 변환하는 프로그램입니다.

```
소스 코드 → [렉서] → 토큰 → [파서] → AST → [코드 생성] → 실행 파일
```

### 1.2 컴파일 과정

```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│  소스 코드   │────▶│    렉서     │────▶│    파서     │
│  (문자열)    │     │ (토큰 생성)  │     │ (구조 분석)  │
└─────────────┘     └─────────────┘     └─────────────┘
                                               │
                          ┌────────────────────┘
                          ▼
                    ┌─────────────┐
                    │    AST      │
                    │ (추상 구문   │
                    │   트리)      │
                    └─────────────┘
```

**각 단계 설명:**

| 단계 | 입력 | 출력 | 역할 |
|------|------|------|------|
| 렉서 (Lexer) | 문자열 | 토큰 배열 | 문자들을 의미 있는 단위(토큰)로 묶음 |
| 파서 (Parser) | 토큰 배열 | AST | 토큰들의 문법적 구조를 분석 |
| 코드 생성 | AST | 실행 코드 | AST를 실행 가능한 코드로 변환 |

### 1.3 예시

소스 코드 `let x := 10 + 20`이 처리되는 과정:

**1. 렉서 출력 (토큰화):**
```
[LET] [ID:"x"] [ASSIGN] [INT:10] [PLUS] [INT:20]
```

**2. 파서 출력 (AST):**
```
LetExp
├── VarDec: x
│   └── init: OpExp(+)
│            ├── IntExp(10)
│            └── IntExp(20)
└── body: ...
```

---

## Part 2: 렉서 (Lexer)

### 2.1 렉서란?

렉서(Lexer, Scanner, Tokenizer)는 소스 코드 문자열을 **토큰(Token)** 단위로 분리하는 프로그램입니다.

**토큰이란?**
- 의미를 가진 최소 단위
- 예: 키워드(`if`, `while`), 식별자(`x`, `myFunc`), 숫자(`123`), 연산자(`+`, `-`)

```
입력: "let x := 10 + 20"

토큰화 결과:
┌─────┬───────┬────────┬─────┬──────┬─────┐
│ LET │ ID(x) │ ASSIGN │ 10  │ PLUS │ 20  │
└─────┴───────┴────────┴─────┴──────┴─────┘
```

### 2.2 토큰 구조

```cpp
// 토큰 종류를 나타내는 열거형
enum class TokenType {
    // 리터럴 (값)
    INT_LIT,        // 정수: 123
    STRING_LIT,     // 문자열: "hello"

    // 식별자 (이름)
    ID,             // 변수명, 함수명: foo, myVar

    // 키워드 (예약어)
    LET, IF, ELSE, WHILE, FOR, FUNCTION, RETURN,

    // 연산자
    PLUS, MINUS, STAR, SLASH,   // + - * /
    EQ, NEQ, LT, LE, GT, GE,    // = <> < <= > >=
    ASSIGN,                      // :=

    // 괄호 및 구두점
    LPAREN, RPAREN,  // ( )
    LBRACE, RBRACE,  // { }
    LBRACK, RBRACK,  // [ ]
    COMMA, SEMI, COLON, DOT,

    // 특수
    END_OF_FILE,     // 파일 끝
    ERROR,           // 오류
};
```

**위치 정보 (오류 메시지용):**
```cpp
struct Position {
    int line;    // 몇 번째 줄
    int column;  // 몇 번째 칸

    Position(int l = 1, int c = 1) : line(l), column(c) {}
};
```

**토큰 구조체:**
```cpp
struct Token {
    TokenType type;      // 토큰 종류
    std::string text;    // 원본 텍스트
    Position pos;        // 소스 코드 내 위치
    int int_value;       // INT_LIT인 경우 숫자 값

    Token(TokenType t, const std::string& txt, Position p)
        : type(t), text(txt), pos(p), int_value(0) {}
};
```

### 2.3 렉서 클래스 구조

```cpp
class Lexer {
public:
    explicit Lexer(const std::string& source);

    Token next_token();     // 다음 토큰 반환 (위치 이동)
    Token peek_token();     // 다음 토큰 확인 (위치 유지)

private:
    std::string source_;    // 소스 코드 전체
    size_t pos_;            // 현재 읽는 위치
    int line_, column_;     // 현재 줄과 칸

    // 문자 단위 연산
    char peek() const;      // 현재 문자 확인 (이동 안 함)
    char peek_next() const; // 다음 문자 확인
    char advance();         // 현재 문자 반환하고 다음으로 이동
    bool at_end() const;    // 소스 끝인지 확인

    // 토큰 스캔 함수들
    Token scan_identifier();  // 식별자/키워드 스캔
    Token scan_number();      // 숫자 스캔
    Token scan_string();      // 문자열 스캔
};
```

### 2.4 핵심 메서드 구현

#### 문자 단위 연산

```cpp
// 현재 위치의 문자를 반환 (이동 안 함)
char Lexer::peek() const {
    if (pos_ >= source_.size()) return '\0';  // 끝이면 널 문자
    return source_[pos_];
}

// 다음 위치의 문자를 반환 (두 글자 연산자용: <=, := 등)
char Lexer::peek_next() const {
    if (pos_ + 1 >= source_.size()) return '\0';
    return source_[pos_ + 1];
}

// 현재 문자를 반환하고 다음 위치로 이동
char Lexer::advance() {
    char c = source_[pos_++];

    // 줄바꿈이면 줄 번호 증가, 칸 번호 초기화
    if (c == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }

    return c;
}

// 소스 끝에 도달했는지 확인
bool Lexer::at_end() const {
    return pos_ >= source_.size();
}
```

**왜 peek()과 advance()를 분리하나요?**
- `peek()`: 다음에 뭐가 올지 "미리 보기"만 함
- `advance()`: 실제로 그 문자를 "소비"하고 다음으로 이동
- 예: `<=`를 처리할 때, `<`를 보고 다음이 `=`인지 확인해야 함

#### 식별자와 키워드 스캔

```cpp
// 키워드 목록
static const std::unordered_map<std::string, TokenType> keywords = {
    {"let",      TokenType::LET},
    {"if",       TokenType::IF},
    {"else",     TokenType::ELSE},
    {"while",    TokenType::WHILE},
    {"function", TokenType::FUNCTION},
    // ... 더 많은 키워드
};

Token Lexer::scan_identifier() {
    Position start(line_, column_);  // 시작 위치 저장
    std::string text;

    // 알파벳, 숫자, 밑줄이 계속되는 동안 읽기
    // 식별자 규칙: [a-zA-Z_][a-zA-Z0-9_]*
    while (!at_end() && (std::isalnum(peek()) || peek() == '_')) {
        text += advance();
    }

    // 키워드인지 확인
    auto it = keywords.find(text);
    if (it != keywords.end()) {
        return Token(it->second, text, start);  // 키워드 토큰
    }
    return Token(TokenType::ID, text, start);   // 식별자 토큰
}
```

**키워드 vs 식별자:**
- `if` → 키워드 (예약어)
- `ifSomething` → 식별자 (변수명)
- 먼저 전체를 읽고, 키워드 목록에서 찾아봄

#### 숫자 스캔

```cpp
Token Lexer::scan_number() {
    Position start(line_, column_);
    std::string text;

    // 숫자가 계속되는 동안 읽기
    while (!at_end() && std::isdigit(peek())) {
        text += advance();
    }

    Token tok(TokenType::INT_LIT, text, start);
    tok.int_value = std::stoi(text);  // 문자열을 숫자로 변환
    return tok;
}
```

#### 문자열 스캔 (이스케이프 시퀀스 처리)

```cpp
Token Lexer::scan_string() {
    Position start(line_, column_);
    advance();  // 여는 따옴표(") 소비

    std::string value;  // 처리된 문자열 (이스케이프 해석 후)

    while (!at_end() && peek() != '"') {
        if (peek() == '\\') {
            // 이스케이프 시퀀스 처리
            advance();  // '\' 소비
            if (at_end()) {
                // 오류: 문자열이 끝나지 않음
                return Token(TokenType::ERROR, value, start);
            }
            char escaped = advance();
            switch (escaped) {
                case 'n':  value += '\n'; break;  // \n → 줄바꿈
                case 't':  value += '\t'; break;  // \t → 탭
                case '\\': value += '\\'; break;  // \\ → 백슬래시
                case '"':  value += '"';  break;  // \" → 따옴표
                default:
                    // 알 수 없는 이스케이프
                    value += escaped;
            }
        } else if (peek() == '\n') {
            // 오류: 문자열 안에서 줄바꿈
            break;
        } else {
            value += advance();
        }
    }

    if (at_end() || peek() != '"') {
        // 오류: 닫는 따옴표가 없음
        return Token(TokenType::ERROR, value, start);
    }

    advance();  // 닫는 따옴표(") 소비
    return Token(TokenType::STRING_LIT, value, start);
}
```

#### 메인 토큰 디스패치

```cpp
Token Lexer::next_token() {
    // 공백과 주석 건너뛰기
    skip_whitespace_and_comments();

    if (at_end()) {
        return Token(TokenType::END_OF_FILE, "", Position(line_, column_));
    }

    Position start(line_, column_);
    char c = peek();

    // 식별자 또는 키워드 (문자로 시작)
    if (std::isalpha(c) || c == '_') {
        return scan_identifier();
    }

    // 숫자 리터럴
    if (std::isdigit(c)) {
        return scan_number();
    }

    // 문자열 리터럴
    if (c == '"') {
        return scan_string();
    }

    // 연산자 및 구두점
    advance();  // 현재 문자 소비

    switch (c) {
        // 단일 문자 연산자
        case '+': return Token(TokenType::PLUS, "+", start);
        case '-': return Token(TokenType::MINUS, "-", start);
        case '*': return Token(TokenType::STAR, "*", start);
        case '/': return Token(TokenType::SLASH, "/", start);
        case '(': return Token(TokenType::LPAREN, "(", start);
        case ')': return Token(TokenType::RPAREN, ")", start);

        // 두 글자가 될 수 있는 연산자
        case '<':
            if (peek() == '=') {
                advance();  // '=' 소비
                return Token(TokenType::LE, "<=", start);
            }
            if (peek() == '>') {
                advance();  // '>' 소비
                return Token(TokenType::NEQ, "<>", start);
            }
            return Token(TokenType::LT, "<", start);

        case ':':
            if (peek() == '=') {
                advance();
                return Token(TokenType::ASSIGN, ":=", start);
            }
            return Token(TokenType::COLON, ":", start);

        // ... 더 많은 연산자

        default:
            return Token(TokenType::ERROR, std::string(1, c), start);
    }
}
```

### 2.5 주석 처리

#### 한 줄 주석 (// ...)

```cpp
void Lexer::skip_line_comment() {
    // '//'를 이미 확인한 후 호출됨
    while (!at_end() && peek() != '\n') {
        advance();
    }
}
```

#### 블록 주석 (/* ... */)

```cpp
void Lexer::skip_block_comment() {
    advance(); // '/' 소비
    advance(); // '*' 소비

    while (!at_end()) {
        if (peek() == '*' && peek_next() == '/') {
            advance(); // '*' 소비
            advance(); // '/' 소비
            return;
        }
        advance();
    }
    // 주석이 닫히지 않음 (오류)
}
```

#### 중첩 주석 (Tiger 스타일: /* /* */ */ 가능)

```cpp
bool Lexer::skip_nested_comment() {
    if (peek() != '/' || peek_next() != '*') return false;

    advance(); advance();  // '/*' 소비

    int depth = 1;  // 중첩 깊이
    while (!at_end() && depth > 0) {
        if (peek() == '/' && peek_next() == '*') {
            advance(); advance();
            depth++;  // 중첩 시작
        } else if (peek() == '*' && peek_next() == '/') {
            advance(); advance();
            depth--;  // 중첩 끝
        } else {
            advance();
        }
    }

    return depth == 0;  // 제대로 닫혔는지
}
```

---

## Part 3: 파서 (Parser)

### 3.1 파서란?

파서는 토큰 스트림을 받아서 **문법 구조**를 분석하고, **AST(추상 구문 트리)**를 생성합니다.

```
토큰: [IF] [ID:x] [GT] [INT:0] [THEN] [ID:x] [ELSE] [INT:0]

파싱 결과 (AST):
IfExp
├── test: OpExp(>)
│         ├── VarExp(x)
│         └── IntExp(0)
├── then: VarExp(x)
└── else: IntExp(0)
```

### 3.2 문맥 자유 문법 (Context-Free Grammar)

언어의 문법은 **생성 규칙**으로 정의됩니다:

```
# Tiger 언어의 일부 문법 (BNF 표기)

program     → expression EOF

expression  → term (('+' | '-') term)*
term        → factor (('*' | '/') factor)*
factor      → NUMBER | '(' expression ')'

if_exp      → 'if' expression 'then' expression ('else' expression)?
while_exp   → 'while' expression 'do' expression
let_exp     → 'let' declarations 'in' expressions 'end'
```

**읽는 법:**
- `→` : "~는 ~로 정의된다"
- `|` : "또는"
- `*` : "0번 이상 반복"
- `?` : "선택적 (있거나 없거나)"
- `'if'` : 리터럴 토큰

### 3.3 재귀 하강 파서 (Recursive Descent Parser)

**재귀 하강 파싱**은 문법의 각 규칙을 함수로 구현하는 기법입니다.

```cpp
class Parser {
public:
    explicit Parser(Lexer& lexer);
    std::unique_ptr<Program> parse();

private:
    Lexer& lexer_;
    Token current_;          // 현재 토큰

    // 토큰 처리 메서드
    Token peek();            // 현재 토큰 확인
    Token advance();         // 토큰 소비하고 다음으로
    bool check(TokenType t); // 현재 토큰이 t인지
    bool match(TokenType t); // t이면 소비하고 true, 아니면 false
    void expect(TokenType t, const char* msg);  // t가 아니면 오류

    // 각 문법 규칙에 대응하는 파싱 함수
    ExpPtr parse_expression();
    ExpPtr parse_if();
    ExpPtr parse_while();
    // ...
};
```

### 3.4 토큰 처리 메서드

```cpp
// 현재 토큰 반환 (소비하지 않음)
Token Parser::peek() {
    return current_;
}

// 현재 토큰 반환하고 다음 토큰으로 이동
Token Parser::advance() {
    Token prev = current_;
    current_ = lexer_.next_token();
    return prev;
}

// 현재 토큰이 특정 타입인지 확인
bool Parser::check(TokenType type) {
    return current_.type == type;
}

// 현재 토큰이 특정 타입이면 소비하고 true, 아니면 false
bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

// 특정 토큰이 반드시 있어야 함. 없으면 오류
void Parser::expect(TokenType type, const char* msg) {
    if (!match(type)) {
        error(msg);  // "expected ')'" 같은 메시지
    }
}
```

**peek vs advance vs match:**
- `peek()`: "다음에 뭐가 오는지 보기만 함"
- `advance()`: "현재 토큰을 가져가고 다음으로"
- `match()`: "이 토큰이면 가져가고 true, 아니면 가만히 있고 false"
- `expect()`: "이 토큰이 반드시 와야 함, 아니면 오류"

### 3.5 연산자 우선순위 (Operator Precedence)

수학에서 `*`가 `+`보다 먼저 계산되듯, 프로그래밍 언어도 연산자 우선순위가 있습니다.

```
1 + 2 * 3 = 1 + (2 * 3) = 7  (곱셈이 먼저)
(1 + 2) * 3 = 9              (괄호가 우선순위 변경)
```

**우선순위 표 (낮은 것 → 높은 것):**

| 순위 | 연산자 | 결합 방향 |
|------|--------|-----------|
| 1 | `:=` (대입) | 오른쪽 |
| 2 | `\|` (논리 OR) | 왼쪽 |
| 3 | `&` (논리 AND) | 왼쪽 |
| 4 | `= <> < <= > >=` (비교) | 왼쪽 |
| 5 | `+ -` (덧셈/뺄셈) | 왼쪽 |
| 6 | `* /` (곱셈/나눗셈) | 왼쪽 |
| 7 | `-` (단항 마이너스) | 오른쪽 |

#### 우선순위 구현: 각 레벨마다 함수

```cpp
// 낮은 우선순위 = 먼저 호출 (바깥쪽)
// 높은 우선순위 = 나중에 호출 (안쪽)

ExpPtr Parser::parse_expression() {
    return parse_assignment();  // 가장 낮은 우선순위부터
}

ExpPtr Parser::parse_assignment() {
    ExpPtr left = parse_or();

    if (match(TokenType::ASSIGN)) {
        // 오른쪽 결합: 재귀 호출
        ExpPtr right = parse_assignment();
        return make_assign(left, right);
    }
    return left;
}

ExpPtr Parser::parse_or() {
    ExpPtr left = parse_and();

    while (match(TokenType::OR)) {
        ExpPtr right = parse_and();
        left = make_or(left, right);  // 왼쪽 결합: 반복
    }
    return left;
}

// ... parse_and, parse_comparison 등 ...

ExpPtr Parser::parse_additive() {
    ExpPtr left = parse_multiplicative();

    while (true) {
        Op op;
        if (match(TokenType::PLUS)) {
            op = Op::PLUS;
        } else if (match(TokenType::MINUS)) {
            op = Op::MINUS;
        } else {
            break;
        }

        ExpPtr right = parse_multiplicative();
        // 왼쪽 결합: left = (left op right)
        left = std::make_unique<OpExp>(std::move(left), op, std::move(right), pos);
    }

    return left;
}

ExpPtr Parser::parse_multiplicative() {
    ExpPtr left = parse_unary();

    while (true) {
        Op op;
        if (match(TokenType::STAR)) {
            op = Op::TIMES;
        } else if (match(TokenType::SLASH)) {
            op = Op::DIVIDE;
        } else {
            break;
        }

        ExpPtr right = parse_unary();
        left = std::make_unique<OpExp>(std::move(left), op, std::move(right), pos);
    }

    return left;
}

ExpPtr Parser::parse_unary() {
    // 단항 마이너스: -expr
    if (match(TokenType::MINUS)) {
        ExpPtr operand = parse_unary();  // 오른쪽 결합: 재귀
        // -x를 (0 - x)로 표현
        return std::make_unique<OpExp>(
            std::make_unique<IntExp>(0, pos),
            Op::MINUS,
            std::move(operand),
            pos
        );
    }

    return parse_primary();  // 가장 높은 우선순위
}
```

**왜 낮은 우선순위가 먼저인가요?**

`1 + 2 * 3`을 파싱할 때:
1. `parse_additive()` 호출
2. `parse_multiplicative()` 호출 → `2 * 3` 처리 → `OpExp(*, 2, 3)` 반환
3. `+`를 만남
4. 다시 `parse_multiplicative()` 호출 → `1` 반환
5. 결과: `OpExp(+, 1, OpExp(*, 2, 3))`

**왼쪽 결합 vs 오른쪽 결합:**

왼쪽 결합 (대부분의 연산자):
```
10 - 5 - 3 = (10 - 5) - 3 = 2
```
구현: `while` 루프 사용

오른쪽 결합 (대입 연산자):
```
a = b = c = a = (b = c)
```
구현: 재귀 호출 사용

### 3.6 기본 표현식 파싱 (Primary)

가장 단순한 표현식들을 처리합니다:

```cpp
ExpPtr Parser::parse_primary() {
    Position pos = current_.pos;

    // nil 리터럴
    if (match(TokenType::NIL)) {
        return std::make_unique<NilExp>(pos);
    }

    // 정수 리터럴
    if (check(TokenType::INT_LIT)) {
        Token tok = advance();
        return std::make_unique<IntExp>(tok.int_value, pos);
    }

    // 문자열 리터럴
    if (check(TokenType::STRING_LIT)) {
        Token tok = advance();
        return std::make_unique<StringExp>(tok.text, pos);
    }

    // 괄호 표현식
    if (match(TokenType::LPAREN)) {
        ExpPtr exp = parse_expression();  // 괄호 안 표현식 파싱
        expect(TokenType::RPAREN, "expected ')'");  // 닫는 괄호 필수
        return exp;
    }

    // if 표현식
    if (check(TokenType::IF)) {
        return parse_if();
    }

    // while 표현식
    if (check(TokenType::WHILE)) {
        return parse_while();
    }

    // 식별자 (변수, 함수 호출, 레코드 생성 등)
    if (check(TokenType::ID)) {
        return parse_id_expression();
    }

    error("expected expression");
    return std::make_unique<NilExp>(pos);  // 오류 복구
}
```

### 3.7 제어문 파싱

#### if 표현식

```
문법: if_exp → 'if' expression 'then' expression ('else' expression)?
예시: if x > 0 then x else -x
```

```cpp
ExpPtr Parser::parse_if() {
    Position pos = current_.pos;
    expect(TokenType::IF, "expected 'if'");

    ExpPtr condition = parse_expression();  // 조건
    expect(TokenType::THEN, "expected 'then'");
    ExpPtr then_branch = parse_expression();  // then 절

    ExpPtr else_branch = nullptr;
    if (match(TokenType::ELSE)) {  // else는 선택적
        else_branch = parse_expression();
    }

    return std::make_unique<IfExp>(
        std::move(condition),
        std::move(then_branch),
        std::move(else_branch),
        pos
    );
}
```

#### while 표현식

```
문법: while_exp → 'while' expression 'do' expression
예시: while i < 10 do i := i + 1
```

```cpp
ExpPtr Parser::parse_while() {
    Position pos = current_.pos;
    expect(TokenType::WHILE, "expected 'while'");

    ExpPtr condition = parse_expression();
    expect(TokenType::DO, "expected 'do'");
    ExpPtr body = parse_expression();

    return std::make_unique<WhileExp>(
        std::move(condition),
        std::move(body),
        pos
    );
}
```

#### for 표현식

```
문법: for_exp → 'for' ID ':=' expression 'to' expression 'do' expression
예시: for i := 0 to 10 do print(i)
```

```cpp
ExpPtr Parser::parse_for() {
    Position pos = current_.pos;
    expect(TokenType::FOR, "expected 'for'");

    if (!check(TokenType::ID)) {
        error("expected identifier");
    }
    std::string var = advance().text;  // 루프 변수

    expect(TokenType::ASSIGN, "expected ':='");
    ExpPtr lo = parse_expression();  // 시작값

    expect(TokenType::TO, "expected 'to'");
    ExpPtr hi = parse_expression();  // 끝값

    expect(TokenType::DO, "expected 'do'");
    ExpPtr body = parse_expression();  // 본문

    return std::make_unique<ForExp>(
        var,
        std::move(lo),
        std::move(hi),
        std::move(body),
        pos
    );
}
```

### 3.8 식별자 표현식 파싱

식별자로 시작하는 표현식은 여러 가지가 될 수 있습니다:
- 단순 변수: `x`
- 함수 호출: `foo(1, 2)`
- 레코드 생성: `Point{x = 1, y = 2}`
- 배열 생성: `intArray[10] of 0`
- 배열/레코드 접근: `arr[i].field`

```cpp
ExpPtr Parser::parse_id_expression() {
    Position pos = current_.pos;
    std::string name = advance().text;  // 식별자 이름

    // 함수 호출: name(args)
    if (check(TokenType::LPAREN)) {
        advance();  // '(' 소비
        std::vector<ExpPtr> args;

        if (!check(TokenType::RPAREN)) {
            args.push_back(parse_expression());
            while (match(TokenType::COMMA)) {
                args.push_back(parse_expression());
            }
        }

        expect(TokenType::RPAREN, "expected ')'");
        return std::make_unique<CallExp>(name, std::move(args), pos);
    }

    // 레코드 생성: name{field = exp, ...}
    if (check(TokenType::LBRACE)) {
        advance();  // '{' 소비
        std::vector<Field> fields;

        if (!check(TokenType::RBRACE)) {
            std::string field_name = advance().text;
            expect(TokenType::EQ, "expected '='");
            fields.emplace_back(field_name, parse_expression());

            while (match(TokenType::COMMA)) {
                field_name = advance().text;
                expect(TokenType::EQ, "expected '='");
                fields.emplace_back(field_name, parse_expression());
            }
        }

        expect(TokenType::RBRACE, "expected '}'");
        return std::make_unique<RecordExp>(name, std::move(fields), pos);
    }

    // 배열 생성 또는 배열 접근: name[exp]
    if (check(TokenType::LBRACK)) {
        advance();  // '[' 소비
        ExpPtr index = parse_expression();
        expect(TokenType::RBRACK, "expected ']'");

        // 배열 생성: name[size] of init
        if (match(TokenType::OF)) {
            ExpPtr init = parse_expression();
            return std::make_unique<ArrayExp>(name, std::move(index), std::move(init), pos);
        }

        // 배열 접근 (계속해서 .field나 [index]가 올 수 있음)
        VarPtr var = std::make_unique<SimpleVar>(name, pos);
        var = std::make_unique<SubscriptVar>(std::move(var), std::move(index), pos);
        var = parse_lvalue_suffix(std::move(var));
        return std::make_unique<VarExp>(std::move(var), pos);
    }

    // 단순 변수 (필드 접근이 이어질 수 있음)
    VarPtr var = std::make_unique<SimpleVar>(name, pos);
    if (check(TokenType::DOT) || check(TokenType::LBRACK)) {
        var = parse_lvalue_suffix(std::move(var));
    }
    return std::make_unique<VarExp>(std::move(var), pos);
}
```

### 3.9 오류 처리

```cpp
void Parser::error(const std::string& msg) {
    std::ostringstream oss;
    oss << current_.pos.line << ":" << current_.pos.column
        << ": 오류: " << msg
        << " (현재 토큰: " << token_type_to_string(current_.type) << ")";
    errors_.push_back(oss.str());
}

// 오류 복구: 안전한 지점까지 토큰 건너뛰기
void Parser::synchronize() {
    while (!check(TokenType::END_OF_FILE)) {
        // 새 문장이 시작되는 키워드를 만나면 중단
        switch (current_.type) {
            case TokenType::LET:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::FOR:
            case TokenType::FUNCTION:
                return;
            default:
                advance();
        }
    }
}
```

---

## Part 4: AST (추상 구문 트리)

### 4.1 AST란?

AST(Abstract Syntax Tree)는 소스 코드의 구조를 트리 형태로 표현한 것입니다.
"추상"이라는 말은 괄호, 세미콜론 같은 문법적 세부사항을 생략했다는 의미입니다.

```
소스: 1 + 2 * 3

AST:
    OpExp(+)
    ├── IntExp(1)
    └── OpExp(*)
        ├── IntExp(2)
        └── IntExp(3)
```

### 4.2 AST 노드 설계

```cpp
// 표현식 종류
enum class ExpKind {
    VAR,      // 변수 참조
    NIL,      // nil 값
    INT,      // 정수 리터럴
    STRING,   // 문자열 리터럴
    CALL,     // 함수 호출
    OP,       // 이항 연산
    RECORD,   // 레코드 생성
    SEQ,      // 순차 실행
    ASSIGN,   // 대입
    IF,       // 조건문
    WHILE,    // while 루프
    FOR,      // for 루프
    BREAK,    // break
    LET,      // let 표현식
    ARRAY,    // 배열 생성
};

// 기본 표현식 구조체
struct Exp {
    ExpKind kind;   // 표현식 종류
    Position pos;   // 소스 코드 위치 (오류 메시지용)

    Exp(ExpKind k, Position p) : kind(k), pos(p) {}
    virtual ~Exp() = default;
};

// 스마트 포인터 별칭
using ExpPtr = std::unique_ptr<Exp>;
```

### 4.3 각 노드 타입

```cpp
// 정수 리터럴
struct IntExp : Exp {
    int value;

    IntExp(int v, Position p)
        : Exp(ExpKind::INT, p), value(v) {}
};

// 이항 연산
struct OpExp : Exp {
    ExpPtr left;    // 왼쪽 피연산자
    Op op;          // 연산자
    ExpPtr right;   // 오른쪽 피연산자

    OpExp(ExpPtr l, Op o, ExpPtr r, Position p)
        : Exp(ExpKind::OP, p),
          left(std::move(l)),
          op(o),
          right(std::move(r)) {}
};

// If 표현식
struct IfExp : Exp {
    ExpPtr test;       // 조건
    ExpPtr then_exp;   // then 절
    ExpPtr else_exp;   // else 절 (nullptr 가능)

    IfExp(ExpPtr t, ExpPtr th, ExpPtr el, Position p)
        : Exp(ExpKind::IF, p),
          test(std::move(t)),
          then_exp(std::move(th)),
          else_exp(std::move(el)) {}
};

// 함수 호출
struct CallExp : Exp {
    std::string func;        // 함수 이름
    std::vector<ExpPtr> args; // 인자들

    CallExp(const std::string& f, std::vector<ExpPtr> a, Position p)
        : Exp(ExpKind::CALL, p),
          func(f),
          args(std::move(a)) {}
};
```

### 4.4 스마트 포인터 사용 이유

```cpp
// 잘못된 방법: 원시 포인터 사용
Exp* left = new IntExp(1, pos);
Exp* right = new IntExp(2, pos);
Exp* add = new OpExp(left, Op::PLUS, right, pos);
// 문제: delete를 잊으면 메모리 누수!

// 올바른 방법: unique_ptr 사용
ExpPtr left = std::make_unique<IntExp>(1, pos);
ExpPtr right = std::make_unique<IntExp>(2, pos);
ExpPtr add = std::make_unique<OpExp>(
    std::move(left),  // 소유권 이전
    Op::PLUS,
    std::move(right), // 소유권 이전
    pos
);
// 장점: add가 소멸될 때 하위 노드들도 자동 해제
```

**std::move()가 필요한 이유:**
- `unique_ptr`는 복사할 수 없음 (하나의 소유자만 가능)
- `std::move()`로 소유권을 다른 포인터로 이전
- 이전 후 원래 포인터는 nullptr가 됨

### 4.5 AST 출력 (디버깅용)

```cpp
class AstPrinter {
public:
    void print(const Exp& exp, int indent = 0) {
        std::string prefix(indent, ' ');  // 들여쓰기

        switch (exp.kind) {
            case ExpKind::INT: {
                auto& e = static_cast<const IntExp&>(exp);
                std::cout << prefix << "IntExp: " << e.value << "\n";
                break;
            }
            case ExpKind::OP: {
                auto& e = static_cast<const OpExp&>(exp);
                std::cout << prefix << "OpExp: " << op_to_string(e.op) << "\n";
                print(*e.left, indent + 2);   // 재귀적으로 출력
                print(*e.right, indent + 2);
                break;
            }
            case ExpKind::IF: {
                auto& e = static_cast<const IfExp&>(exp);
                std::cout << prefix << "IfExp:\n";
                std::cout << prefix << "  test:\n";
                print(*e.test, indent + 4);
                std::cout << prefix << "  then:\n";
                print(*e.then_exp, indent + 4);
                if (e.else_exp) {
                    std::cout << prefix << "  else:\n";
                    print(*e.else_exp, indent + 4);
                }
                break;
            }
            // ... 다른 케이스들
        }
    }
};
```

---

## 요약

### 렉서 핵심

| 함수 | 역할 |
|------|------|
| `peek()` | 현재 문자 확인 (이동 안 함) |
| `advance()` | 현재 문자 반환하고 다음으로 이동 |
| `scan_identifier()` | 식별자/키워드 스캔 |
| `scan_number()` | 숫자 스캔 |
| `scan_string()` | 문자열 스캔 (이스케이프 처리) |

### 파서 핵심

| 함수 | 역할 |
|------|------|
| `peek()` | 현재 토큰 확인 |
| `advance()` | 토큰 소비하고 다음으로 |
| `check(type)` | 현재 토큰이 type인지 확인 |
| `match(type)` | type이면 소비하고 true |
| `expect(type, msg)` | type이 아니면 오류 |

### 우선순위 규칙

1. 낮은 우선순위 함수가 높은 우선순위 함수를 호출
2. 왼쪽 결합: `while` 루프 사용
3. 오른쪽 결합: 재귀 호출 사용

### AST 규칙

1. 각 노드 타입은 `kind` 필드로 구분
2. 자식 노드는 `unique_ptr`로 소유
3. `std::move()`로 소유권 이전
4. `Position` 필드로 소스 위치 추적
