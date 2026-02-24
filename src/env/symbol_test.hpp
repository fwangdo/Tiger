#ifndef TIGER_SYMBOL_HPP
#define TIGER_SYMBOL_HPP

#include <string>  
#include <unordered_set> 
#include <unordered_map> 
#include <vector> 

namespace tiger {

class Symbol {
public:
  // interning
  static const std::string *intern(const std::string &name) {
    auto [it, _] = pool_.insert(name); 
    return &(*it); 
  }; 
  // get name
  static const std::string &name(const std::string *sym) {
    return *sym;  
  };   
private:
  static std::unordered_set<std::string> pool_; 
}; 

template <typename Value>
class SymbolTable {
public:
  void enter(const std::string *sym, Value *value) {
    bindings_[sym].push_back(value); 
    undo_stack_.push_back(sym); 
  }

  Value *look(const std::string *sym) const {
    auto it = bindings_.find(sym);
    if (it != bindings_.end() && !it->second.empty()) {
      return it->second.back(); 
    }
  }

  void beginScope() {
    undo_stack_.push_back(nullptr); 
  }

  void endScope() {
    while (!undo_stack_.empty()) {
      auto sym = undo_stack_.back(); 
      undo_stack_.pop_back(); 
      if (!sym) break; // which is begin scope.  
      bindings_[sym].pop_back(); 
    }
  }
private: 
  std::unordered_map<const std::string*, std::vector<Value*>> bindings_; 

  // undo stack. 
  std::vector<const std::string*> undo_stack_;
}; 

}

#endif