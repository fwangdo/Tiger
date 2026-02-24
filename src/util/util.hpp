#ifndef TIGER_UTIL_HPP
#define TIGER_UTIL_HPP

#include <functional>  // std::hash
#include <string>

namespace tiger::util {

// Wraps std::hash<std::string> for use across EnvTable, Symbol, etc.
inline std::size_t hash(const std::string& key) {
  return std::hash<std::string>{}(key);
}

} // namespace tiger::util

#endif // TIGER_UTIL_HPP
