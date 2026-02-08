# Tiger 컴파일러 학습 진행표

## 학습 목표
- Modern C++ (C++17/20) 패턴 습득
- CMake 빌드 시스템 마스터
- 셸 스크립트 자동화
- 컴파일러 전체 파이프라인 구현

---

## 완료된 장

### ✅ Chapter 3: Lexical Analysis
- [x] 토큰 타입 정의 (`token.h`)
- [x] Lexer 클래스 구현 (`lexer.cpp`)
- [x] 정규표현식 기반 스캐닝
- [x] 위치 추적 (line, column)

### ✅ Chapter 4: Parsing
- [x] 재귀 하강 파서 구현 (`parser.cpp`)
- [x] 연산자 우선순위 처리
- [x] 에러 복구 기본 구현

### ✅ Chapter 5: Abstract Syntax
- [x] AST 노드 정의 (`ast.h`)
- [x] unique_ptr 기반 메모리 관리
- [x] AST 출력기 (`ast_printer.cpp`)

---

## Chapter 2: Introduction

### 학습 목표
- 컴파일러 전체 구조 이해
- 모듈 분리 설계
- 빌드 시스템 구축

### 할 일

#### 2.1 프로젝트 구조 정리
- [ ] 디렉토리 구조 재정비
  ```
  tiger/
  ├── src/           # 소스 코드
  │   ├── lexer/
  │   ├── parser/
  │   ├── ast/
  │   ├── semantic/  # 추가 예정
  │   ├── ir/        # 추가 예정
  │   └── codegen/   # 추가 예정
  ├── include/       # 헤더 파일
  ├── tests/         # 테스트
  ├── examples/      # Tiger 예제 코드
  └── docs/          # 문서
  ```

#### 2.2 CMake 고급 설정
- [ ] 모듈별 라이브러리 분리
  ```cmake
  # 각 모듈을 별도 라이브러리로
  add_library(tiger_lexer STATIC src/lexer/lexer.cpp)
  add_library(tiger_parser STATIC src/parser/parser.cpp)
  add_library(tiger_semantic STATIC src/semantic/semant.cpp)
  ```
- [ ] 테스트 빌드 설정 (CTest 또는 Google Test)
  ```cmake
  enable_testing()
  add_executable(test_lexer tests/test_lexer.cpp)
  add_test(NAME LexerTest COMMAND test_lexer)
  ```
- [ ] 설치 규칙 작성
  ```cmake
  install(TARGETS tiger DESTINATION bin)
  ```

#### 2.3 셸 스크립트 자동화
- [ ] `scripts/test.sh` - 전체 테스트 실행
  ```bash
  #!/bin/bash
  set -euo pipefail
  cd "$(dirname "$0")/.."
  cmake --build build --target test
  ```
- [ ] `scripts/format.sh` - 코드 포맷팅 (clang-format)
- [ ] `scripts/lint.sh` - 정적 분석 (clang-tidy)

#### 2.4 Modern C++ 적용
- [ ] `std::string_view` 사용 (불필요한 복사 제거)
- [ ] `std::optional` 사용 (nullable 반환값)
- [ ] `std::variant` 사용 (타입 안전 유니온)

### 실습 과제
1. 현재 프로젝트를 모듈별 라이브러리로 분리
2. `build.sh`에 테스트 실행 옵션 추가
3. GitHub Actions CI 설정 파일 작성

---

## Chapter 6: Type Checking

### 학습 목표
- 심볼 테이블 구현
- 타입 시스템 설계
- 의미 분석 (Semantic Analysis)

### 할 일

#### 6.1 심볼 테이블 구현
- [ ] `include/symbol.h` 생성
  ```cpp
  // Modern C++ 패턴: std::unordered_map + std::optional
  class SymbolTable {
      std::unordered_map<std::string, Symbol> symbols;
      SymbolTable* parent;  // 스코프 체인
  public:
      std::optional<Symbol> lookup(std::string_view name);
      void insert(std::string name, Symbol sym);
      void beginScope();
      void endScope();
  };
  ```

#### 6.2 타입 정의
- [ ] `include/types.h` 생성
  ```cpp
  // std::variant로 타입 안전하게 표현
  using Type = std::variant<
      IntType,
      StringType,
      ArrayType,
      RecordType,
      FunctionType
  >;
  ```

#### 6.3 타입 검사기 구현
- [ ] `src/semantic/type_checker.cpp`
- [ ] 표현식 타입 추론
- [ ] 선언문 타입 검사
- [ ] 타입 호환성 검사

#### 6.4 에러 리포팅
- [ ] 상세한 에러 메시지
  ```cpp
  // 위치 정보 + 컨텍스트 포함
  error(pos, "type mismatch: expected {}, got {}", expected, actual);
  ```

### 실습 과제
1. 간단한 심볼 테이블 테스트 작성
2. 정수/문자열 리터럴 타입 검사 구현
3. 변수 선언 및 참조 타입 검사

### 빌드 시스템 추가
- [ ] `src/semantic/CMakeLists.txt` 작성
- [ ] 메인 CMakeLists.txt에 서브디렉토리 추가

---

## Chapter 7: Activation Records

### 학습 목표
- 함수 호출 규약 이해
- 스택 프레임 구조
- 지역 변수 관리

### 할 일

#### 7.1 프레임 추상화
- [ ] `include/frame.h` 생성
  ```cpp
  // 플랫폼 독립적 프레임 인터페이스
  class Frame {
  public:
      virtual ~Frame() = default;
      virtual Access allocLocal(bool escape) = 0;
      virtual int size() const = 0;
  };
  ```

#### 7.2 x86-64 프레임 구현
- [ ] `src/frame/x86_64_frame.cpp`
- [ ] 레지스터 할당 규칙
- [ ] 스택 레이아웃

#### 7.3 Escape 분석
- [ ] 중첩 함수에서 외부 변수 접근 분석
- [ ] escape 변수는 스택에, 아니면 레지스터에

### 실습 과제
1. 간단한 함수의 스택 프레임 시각화
2. 재귀 함수 호출 시 스택 추적

---

## Chapter 8: Translation to Intermediate Code

### 학습 목표
- 중간 표현 (IR) 설계
- AST → IR 변환
- 트리 기반 IR

### 할 일

#### 8.1 IR 트리 정의
- [ ] `include/ir/tree.h`
  ```cpp
  // IR 표현식
  enum class IrExpKind { CONST, NAME, TEMP, BINOP, MEM, CALL, ESEQ };

  // IR 문장
  enum class IrStmKind { MOVE, EXP, JUMP, CJUMP, SEQ, LABEL };
  ```

#### 8.2 Translate 모듈
- [ ] `src/ir/translate.cpp`
- [ ] 표현식 변환 (Ex, Nx, Cx 구분)
- [ ] 조건문/반복문 변환

#### 8.3 임시값 관리
- [ ] `Temp` 클래스 (가상 레지스터)
- [ ] `Label` 클래스 (점프 목적지)

### 실습 과제
1. 간단한 산술식 IR 변환 테스트
2. if-else 문의 IR 출력 확인

### 셸 스크립트
- [ ] `scripts/dump_ir.sh` - IR 덤프 도구
  ```bash
  #!/bin/bash
  ./build/tiger --ir "$1" | less
  ```

---

## Chapter 9: Basic Blocks and Traces

### 학습 목표
- IR 정규화
- 기본 블록 분할
- 트레이스 생성

### 할 일

#### 9.1 정규 트리 변환
- [ ] ESEQ 제거
- [ ] CALL을 문장 레벨로 이동

#### 9.2 기본 블록 생성
- [ ] `src/ir/basic_block.cpp`
  ```cpp
  struct BasicBlock {
      Label label;
      std::vector<IrStm> stmts;
      // 마지막은 JUMP 또는 CJUMP
  };
  ```

#### 9.3 트레이스 스케줄링
- [ ] 조건 분기 최적화
- [ ] fall-through 활용

---

## Chapter 10: Instruction Selection

### 학습 목표
- 트리 패턴 매칭
- 타일링 알고리즘
- 어셈블리 생성

### 할 일

#### 10.1 명령어 정의
- [ ] `include/codegen/instr.h`
  ```cpp
  struct Instr {
      std::string assem;           // 어셈블리 템플릿
      std::vector<Temp> dst;       // 결과 레지스터
      std::vector<Temp> src;       // 소스 레지스터
      std::optional<Label> jump;   // 점프 목적지
  };
  ```

#### 10.2 x86-64 코드 생성
- [ ] `src/codegen/x86_64_codegen.cpp`
- [ ] Maximal Munch 알고리즘

#### 10.3 어셈블리 출력
- [ ] AT&T 문법 또는 Intel 문법 선택
- [ ] 디버그 정보 포함

### 빌드 시스템
- [ ] 어셈블러(as) 호출 스크립트
- [ ] 링커(ld) 호출 스크립트

---

## Chapter 11: Liveness Analysis

### 학습 목표
- 데이터플로우 분석
- 활성 변수 분석
- 간섭 그래프

### 할 일

#### 11.1 흐름 그래프 구성
- [ ] `src/analysis/flowgraph.cpp`
- [ ] 기본 블록 간 엣지

#### 11.2 활성 분석
- [ ] `src/analysis/liveness.cpp`
- [ ] def/use 집합 계산
- [ ] 고정점 반복

#### 11.3 간섭 그래프
- [ ] `src/analysis/interference.cpp`
- [ ] 동시에 활성인 변수 쌍 기록

---

## Chapter 12: Register Allocation

### 학습 목표
- 그래프 컬러링
- Spilling 처리
- Coalescing

### 할 일

#### 12.1 그래프 컬러링 구현
- [ ] `src/regalloc/color.cpp`
- [ ] Simplify → Select 알고리즘

#### 12.2 Spill 코드 생성
- [ ] 레지스터 부족 시 스택 사용
- [ ] Spill 후 재분석

#### 12.3 Move Coalescing
- [ ] 불필요한 복사 제거
- [ ] Conservative coalescing

---

## Chapter 13: Putting It All Together

### 학습 목표
- 전체 파이프라인 통합
- 실행 파일 생성
- 런타임 라이브러리

### 할 일

#### 13.1 컴파일러 드라이버
- [ ] `src/main.cpp` 확장
  ```cpp
  // 전체 파이프라인
  auto tokens = lexer.scan(source);
  auto ast = parser.parse(tokens);
  semant.check(ast);
  auto ir = translate(ast);
  auto canon = canonicalize(ir);
  auto instrs = codegen(canon);
  auto colored = regalloc(instrs);
  emit(colored, output);
  ```

#### 13.2 런타임 라이브러리
- [ ] `runtime/runtime.c`
  ```c
  // Tiger 표준 함수
  void print(const char* s);
  int ord(const char* s);
  char* chr(int i);
  int size(const char* s);
  char* concat(const char* a, const char* b);
  ```

#### 13.3 빌드 자동화
- [ ] `scripts/compile.sh` - 전체 컴파일 스크립트
  ```bash
  #!/bin/bash
  set -euo pipefail

  INPUT="$1"
  OUTPUT="${2:-a.out}"

  # Tiger → 어셈블리
  ./build/tiger -S "$INPUT" -o "${INPUT%.tig}.s"

  # 어셈블리 → 오브젝트
  as "${INPUT%.tig}.s" -o "${INPUT%.tig}.o"

  # 링크
  ld "${INPUT%.tig}.o" runtime/runtime.o -o "$OUTPUT"
  ```

#### 13.4 테스트 프레임워크
- [ ] `tests/integration/` - 통합 테스트
- [ ] `scripts/run_tests.sh` - 자동 테스트
  ```bash
  #!/bin/bash
  for file in examples/*.tig; do
      echo "Testing $file..."
      ./build/tiger "$file" -o /tmp/test
      /tmp/test > /tmp/output
      diff /tmp/output "${file%.tig}.expected"
  done
  ```

---

## 고급 주제 (선택)

### Chapter 14: Garbage Collection
- [ ] Mark-and-sweep GC 구현
- [ ] 루트 집합 관리

### Chapter 15: Object-Oriented Languages
- [ ] 클래스/메서드 지원
- [ ] 가상 메서드 테이블

### Chapter 16: Functional Programming
- [ ] 클로저 구현
- [ ] 꼬리 호출 최적화

---

## 학습 자료

### Modern C++ 참고
- [ ] cppreference.com - 표준 라이브러리 레퍼런스
- [ ] C++ Core Guidelines - 모범 사례
- [ ] Effective Modern C++ (Scott Meyers)

### 빌드 시스템
- [ ] CMake 공식 튜토리얼
- [ ] Professional CMake (Craig Scott)

### 컴파일러
- [ ] Modern Compiler Implementation in C (Appel)
- [ ] Engineering a Compiler (Cooper & Torczon)

---

## 진행 상황 요약

| 장 | 상태 | 비고 |
|----|------|------|
| Ch 3: Lexical Analysis | ✅ 완료 | |
| Ch 4: Parsing | ✅ 완료 | |
| Ch 5: Abstract Syntax | ✅ 완료 | |
| Ch 2: Introduction | 🔲 예정 | 프로젝트 구조 정비 |
| Ch 6: Type Checking | 🔲 예정 | 심볼 테이블 필요 |
| Ch 7: Activation Records | 🔲 예정 | |
| Ch 8: IR Translation | 🔲 예정 | |
| Ch 9: Basic Blocks | 🔲 예정 | |
| Ch 10: Instruction Selection | 🔲 예정 | |
| Ch 11: Liveness Analysis | 🔲 예정 | |
| Ch 12: Register Allocation | 🔲 예정 | |
| Ch 13: Integration | 🔲 예정 | |

---

## 다음 단계

**권장 순서:**
1. Chapter 2 - 프로젝트 구조 정비 및 빌드 시스템 강화
2. Chapter 6 - 타입 검사 (심볼 테이블 먼저)
3. Chapter 7-8 - IR 생성
4. Chapter 9-12 - 코드 생성 및 최적화
5. Chapter 13 - 통합 및 완성
