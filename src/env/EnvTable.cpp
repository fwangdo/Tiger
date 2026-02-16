#include "EnvTable.hpp"
#include <memory>

namespace tiger {

// ============================================================================
// Constructor
// ============================================================================
//
// C version:
//   struct bucket *table[SIZE];   // global array, zero-initialized
//
// C++: std::vector needs explicit sizing.
//   table_(SIZE)  creates a vector of SIZE null unique_ptrs.

EnvTable::EnvTable() : table_(SIZE) {}

// ============================================================================
// Bucket constructor
// ============================================================================
//
// C version:
//   b->key = key;  b->binding = binding;  b->next = next;
//
// C++: Use a member initializer list.
//   std::unique_ptr cannot be copied — use std::move() to transfer ownership.

EnvTable::Bucket::Bucket(const std::string& k, std::unique_ptr<Binding> b,
                         std::unique_ptr<Bucket> n)
  : key(k), binding(std::move(b)), next(std::move(n)) {}

// ============================================================================
// hash
// ============================================================================
//
// C version:
//   for (s = s0; *s; s++)
//       h = h * 65599 + *s;
//
// C++: range-based for:
//   for (char c : key) { ... }

unsigned int EnvTable::hash(const std::string& key) {
  unsigned int h = 0;
  for (char c : key) {
    h = h * 65599 + static_cast<unsigned int>(c);
  }
  return h;
}

// ============================================================================
// insert
// ============================================================================
//
// C version:
//   int index = hash(key) % SIZE;
//   table[index] = Bucket(key, binding, table[index]);
//
// C++: std::make_unique<Bucket>(key, std::move(binding), std::move(table_[index]))
//      then assign it to table_[index] with std::move.

void EnvTable::insert(const std::string& key, std::unique_ptr<Binding> binding) {
  int index = hash(key) % SIZE;
  // TODO: create a new Bucket and link old head as next
  // TODO: assign it to table_[index]
  // hint: consider data exists already in table_[index], then we should move the data to next slot of generated bucket. 
  table_[index] = std::make_unique<Bucket>(key, std::move(binding), std::move(table_[index])) ; 
}

// ============================================================================
// lookup
// ============================================================================
//
// C version:
//   for (b = table[index]; b; b = b->next)
//       if (0 == strcmp(b->key, key)) return b->binding;
//   return NULL;
//
// C++: Compare strings with ==.
//      Traverse with raw pointer: Bucket* b = table_[index].get();
//      Return binding.get() (raw pointer, caller doesn't take ownership).
//      Return nullptr if not found.

Binding* EnvTable::lookup(const std::string& key) const {
  int index = hash(key) % SIZE; 
  // TODO: traverse the chain, return b->binding.get() when b->key == key
  // .get() method in smart pointer gives raw pointer. 
  for (auto b = table_[index].get(); b; b =b->next.get()) {
    if (b->key == key) {
      return b->binding.get(); 
    }
  }
  return nullptr;
}

// ============================================================================
// pop
// ============================================================================
//
// C version:
//   table[index] = table[index]->next;
//
// C++: std::move the next pointer into table_[index].
//      The old head is automatically freed.

void EnvTable::pop(const std::string& key) {
  int index = hash(key) % SIZE;
  // TODO: move table_[index]->next into table_[index]
  table_[index] = std::move(table_[index]->next);  
}

} // namespace tiger
