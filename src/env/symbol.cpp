#include "symbol.hpp"

namespace tiger {

// ============================================================================
// Symbol 정적 멤버 정의
// ============================================================================
//
// 헤더에서 선언만 한 static 멤버를 여기서 정의.
//   inline static (C++17)을 쓰면 헤더에서 정의 가능하지만,
//   .hpp/.cpp 분리 시에는 전통적 방식이 더 명확함:
//     - 헤더: static std::unordered_set<std::string> pool_;  (선언)
//     - 소스: std::unordered_set<std::string> Symbol::pool_;  (정의)
//
//   이렇게 하면 pool_이 정확히 하나의 번역 단위(translation unit)에 존재.

std::unordered_set<std::string> Symbol::pool_;

// ============================================================================
// intern
// ============================================================================
//
// C 원본: S_Symbol S_Symbol(string name)
//
// unordered_set::insert는 {iterator, bool}을 반환.
//   이미 있으면 기존 원소의 iterator, 없으면 새로 삽입한 iterator.
//   어느 쪽이든 &(*it)로 안정적인 포인터를 얻음.
//
// 구조화된 바인딩(structured binding, C++17):
//   auto [it, inserted] = pool_.insert(name);
//   → std::pair를 it과 inserted 두 변수로 분해.
//   inserted는 사용하지 않으므로 _로 표기 (관례).

const std::string* Symbol::intern(const std::string& name) {
  auto [it, _] = pool_.insert(name);
  return &(*it);
}

// ============================================================================
// name
// ============================================================================
//
// C 원본: string S_name(S_Symbol s) { return s->name; }
//
// intern()이 반환한 포인터를 역참조하여 원래 문자열을 돌려줌.
// const 참조를 반환 → 복사 없음.

const std::string& Symbol::name(const std::string* sym) {
  return *sym;
}

} // namespace tiger
