#include "env/EnvTable.hpp"
#include <cassert>
#include <iostream>

struct IntBinding : tiger::Binding {
  int value;
  IntBinding(int v) : value(v) {}
};

int main() {
  tiger::EnvTable table;

  // 1. lookup on empty table returns nullptr
  assert(table.lookup("x") == nullptr);

  // 2. insert and lookup
  table.insert("x", std::make_unique<IntBinding>(10));
  auto* b = table.lookup("x");
  assert(b != nullptr);
  assert(static_cast<IntBinding*>(b)->value == 10);

  // 3. lookup miss
  assert(table.lookup("y") == nullptr);

  // 4. shadow: insert same key again
  table.insert("x", std::make_unique<IntBinding>(20));
  b = table.lookup("x");
  assert(static_cast<IntBinding*>(b)->value == 20);

  // 5. pop restores previous binding
  table.pop("x");
  b = table.lookup("x");
  assert(static_cast<IntBinding*>(b)->value == 10);

  // 6. pop again removes last binding
  table.pop("x");
  assert(table.lookup("x") == nullptr);

  std::cout << "All EnvTable tests passed!\n";
  return 0;
}
