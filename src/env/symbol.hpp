#ifndef TIGER_SYMBOL_HPP
#define TIGER_SYMBOL_HPP

// ============================================================================
// symbol.hpp — 심볼 인터닝(Symbol interning) + 스코프 심볼 테이블
//
// Tiger 책(Appel)의 symbol.h / symbol.c를 modern C++로 포팅.
//
// 구조:
//   Part 1: Symbol         — 문자열 인터닝 (선언만, 구현은 symbol.cpp)
//   Part 2: SymbolTable<V> — 스코프 심볼 테이블 (템플릿 → 헤더에 구현)
//
// 왜 분리하는가?
//   Symbol: 일반 클래스 → 선언(.hpp)과 구현(.cpp) 분리 가능.
//   SymbolTable: 클래스 템플릿 → 컴파일러가 사용 시점에 인스턴스화해야 하므로
//     구현이 반드시 헤더에 있어야 함. .cpp에 넣으면 링크 에러 발생.
// ============================================================================

#include <string>
#include <unordered_set>   // Symbol 인터닝 풀
#include <unordered_map>   // SymbolTable 바인딩 저장
#include <vector>          // SymbolTable undo 스택 + 값 스택

namespace tiger {

// ============================================================================
// Part 1: Symbol — 문자열 인터닝 (String Interning)
// ============================================================================
//
// 핵심 아이디어:
//   "foo"를 여러 번 intern하면, 항상 같은 const std::string* 포인터를 반환.
//   → 문자열 비교가 포인터 비교(O(1))로 바뀜.
//
// C 원본에서는 struct S_symbol_ + 수동 해시 체이닝이었지만,
// std::unordered_set이 이 모든 걸 해줌:
//   - 해시 계산, 충돌 처리, 동적 리사이징 전부 자동.
//   - 중요: unordered_set은 원소의 포인터가 안정적(stable).
//     즉, insert 후에도 기존 원소의 주소가 변하지 않음.
//     → 인터닝에 딱 맞는 컨테이너.
// ============================================================================

class Symbol {
public:
  // intern: 문자열을 인터닝하여 고유한 const string*를 반환.
  //   C 원본: S_Symbol S_Symbol(string name)
  static const std::string* intern(const std::string& name);

  // name: 심볼에서 원래 문자열을 꺼냄.
  //   C 원본: string S_name(S_Symbol s) { return s->name; }
  static const std::string& name(const std::string* sym);

private:
  // 인터닝 풀: 프로그램 전체에서 하나만 존재 (static).
  //   C 원본: static S_Symbol hashtable[SIZE]; + 수동 체이닝
  //   C++:    unordered_set이 해시/충돌/리사이징 전부 처리.
  static std::unordered_set<std::string> pool_;
};

// ============================================================================
// Part 2: SymbolTable<Value> — 스코프가 있는 심볼 테이블
// ============================================================================
//
// C 원본에서 TAB_table + S_table이 하던 역할을 하나의 클래스 템플릿으로 통합.
//
// C → C++ 주요 변환:
//   C:   void* value                →  C++: Value* (템플릿 파라미터)
//   C:   고정 해시 + binder 체인     →  C++: unordered_map + vector
//   C:   TAB_pop (linked list)      →  C++: vector::pop_back
//
// 스코프 메커니즘:
//   C에서는 linked-list의 LIFO 특성을 이용했지만,
//   C++에서는 두 개의 vector로 더 명확하게 표현:
//
//   1. bindings_: 심볼 → 값 스택 (vector<Value*>)
//      같은 심볼에 여러 값을 바인딩 가능 (섀도잉).
//      back()이 현재 스코프의 값, pop_back()으로 이전 스코프 복원.
//
//   2. undo_stack_: enter 순서를 기록.
//      beginScope()는 nullptr 마커를 push.
//      endScope()는 마커까지 pop하며 bindings_에서도 제거.
//
//   C의 "linked-list를 스택처럼 사용" = C++의 "vector 두 개".
// ============================================================================

template <typename Value>
class SymbolTable {
public:
  // enter: 심볼에 값을 바인딩.
  //   C 원본: void S_enter(S_table t, S_Symbol sym, void *value)
  void enter(const std::string* sym, Value* value) {
    bindings_[sym].push_back(value);
    undo_stack_.push_back(sym);
  }

  // look: 심볼에 바인딩된 값을 검색. 없으면 nullptr.
  //   C 원본: void *S_look(S_table t, S_Symbol sym)
  //
  //   vector의 back()이 가장 최근 바인딩 (현재 스코프).
  Value* look(const std::string* sym) const {
    auto it = bindings_.find(sym);
    if (it != bindings_.end() && !it->second.empty())
      return it->second.back();
    return nullptr;
  }

  // beginScope: 스코프 시작 마커를 삽입.
  //   C 원본: S_beginScope → &marksym를 key로 삽입
  //   C++:    nullptr을 undo_stack에 push (별도 마커 심볼 불필요).
  void beginScope() {
    undo_stack_.push_back(nullptr);
  }

  // endScope: 현재 스코프의 모든 바인딩을 제거.
  //   C 원본: do { s = TAB_pop(t); } while (s != &marksym);
  //
  //   undo_stack을 역순으로 pop하며 bindings_에서도 제거.
  //   nullptr(마커)를 만나면 종료.
  void endScope() {
    while (!undo_stack_.empty()) {
      auto sym = undo_stack_.back();
      undo_stack_.pop_back();
      if (!sym) break;              // 마커 도달 → 스코프 끝
      bindings_[sym].pop_back();
    }
  }

private:
  // bindings_: 심볼 → 값 스택.
  //   vector로 섀도잉 처리: push_back(새 값), pop_back(스코프 해제).
  std::unordered_map<const std::string*, std::vector<Value*>> bindings_;

  // undo_stack_: enter 순서 기록 + 스코프 마커(nullptr).
  //   C의 "top + prevtop 체인"을 하나의 vector로 대체.
  std::vector<const std::string*> undo_stack_;
};

} // namespace tiger

#endif // TIGER_SYMBOL_HPP
