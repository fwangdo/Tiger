#ifndef ENV_TABLE_H
#define ENV_TABLE_H

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstddef>

namespace tiger {

//
// 1. 공통 상위 타입 (Environment Entry)
//
class EnvEntry {
public:
    virtual ~EnvEntry() = default;
};


//
// 2. SymbolTable (runtime polymorphism 기반)
//
class SymbolTable {
public:
    SymbolTable() = default;
    ~SymbolTable() = default;

    // 복사 금지
    SymbolTable(const SymbolTable&) = delete;
    SymbolTable& operator=(const SymbolTable&) = delete;

    // 이동 허용
    SymbolTable(SymbolTable&&) noexcept = default;
    SymbolTable& operator=(SymbolTable&&) noexcept = default;

    void insert(const std::string& key,
                std::unique_ptr<EnvEntry> value);

    EnvEntry* lookup(const std::string& key) const;

    void remove(const std::string& key);

private:
    static constexpr std::size_t SIZE = 109;

    struct Bucket {
        std::string key;
        std::unique_ptr<EnvEntry> value;
        std::unique_ptr<Bucket> next;

        Bucket(const std::string& k,
               std::unique_ptr<EnvEntry> v)
            : key(k),
              value(std::move(v)),
              next(nullptr) {}
    };

    std::vector<std::unique_ptr<Bucket>> table_{SIZE};

    std::size_t hash(const std::string& key) const;
};

} // namespace tiger

#endif
