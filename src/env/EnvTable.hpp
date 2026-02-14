#ifndef TIGER_ENV_TABLE_HPP
#define TIGER_ENV_TABLE_HPP

//  C:   void *binding;             →  C++: std::unique_ptr<Binding> (virtual base)
//  C:   struct bucket *table[109]; →  C++: std::vector<std::unique_ptr<Bucket>>
//  C:   typedef char *string;      →  C++: std::string

#include <memory>    // std::unique_ptr, std::make_unique
#include <string>    // c++ string.
#include <vector>    // standard container.  

namespace tiger {

// Base class for all binding types.
// Derive from this to store any kind of value in the table.
//
//   struct VarBinding : Binding { Type type; int offset; };
//   struct FunBinding : Binding { std::vector<Type> params; Type result; };
//
// virtual destructor ensures derived classes are cleaned up properly.
struct Binding {
    virtual ~Binding() = default;
};

class EnvTable {
public:
    EnvTable();

    void     insert(const std::string& key, std::unique_ptr<Binding> binding);
    Binding* lookup(const std::string& key) const;
    void     pop(const std::string& key);

private:
    static constexpr int SIZE = 109;

    struct Bucket {
        std::string key;
        std::unique_ptr<Binding> binding;
        std::unique_ptr<Bucket> next;

        Bucket(const std::string& k, std::unique_ptr<Binding> b,
               std::unique_ptr<Bucket> n);
    };

    std::vector<std::unique_ptr<Bucket>> table_;

    static unsigned int hash(const std::string& key);
};

} // namespace tiger

#endif // TIGER_ENV_TABLE_HPP
