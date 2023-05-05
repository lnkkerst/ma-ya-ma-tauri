#pragma once
#include "include/Keyword.hpp"
#include "rust/cxx.h"

class Tile {
public:
  Tile();
  Tile(Tile &&) = default;
  Tile(const Tile &) = default;
  Tile &operator=(Tile &&) = default;
  Tile &operator=(const Tile &) = default;
  ~Tile();

  rust::String id;
  rust::i32 row;
  rust::i32 column;
  rust::i32 index;
  bool exposed;
  Keyword keyword;
  bool onBuffer;
  bool dropped;
  bool selected;

private:
};
