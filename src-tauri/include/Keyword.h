#pragma once
#include "rust/cxx.h"

class Keyword {
public:
  Keyword();
  Keyword(Keyword &&) = default;
  Keyword(const Keyword &) = default;
  Keyword &operator=(Keyword &&) = default;
  Keyword &operator=(const Keyword &) = default;
  ~Keyword();

private:
};
