#pragma once

#include <memory>
#include <stdlib.h>
#include <string>

#ifdef __GNUC__

#include <cxxabi.h>

struct deleter_free {
  template <class T> void operator()(T *p) const { free(p); }
};

template <typename T> std::string demangle() {
  const char *name = typeid(T).name();
  int status = 0;
  std::unique_ptr<char, deleter_free> newName(
      abi::__cxa_demangle(name, nullptr, nullptr, &status));
  return (status == 0) ? newName.get() : name;
}

#else

template <typename T> std::string demangle() { return typeid(T).name(); }

#endif
