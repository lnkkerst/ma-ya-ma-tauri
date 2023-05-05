#pragma once
#include <memory>

#include "rust/cxx.h"

class Keyword {
public:
  Keyword();
  Keyword(Keyword &&) = default;
  Keyword(const Keyword &) = default;
  Keyword &operator=(Keyword &&) = default;
  Keyword &operator=(const Keyword &) = default;
  ~Keyword();

  rust::String content;
  std::shared_ptr<rust::String> type;
  std::shared_ptr<rust::String> color;
  rust::String backgroundColor;

private:
};
