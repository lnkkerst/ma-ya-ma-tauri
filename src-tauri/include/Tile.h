#pragma once
#include "rust/cxx.h"

class Tile {
public:
  Tile();
  Tile(Tile &&) = default;
  Tile(const Tile &) = default;
  Tile &operator=(Tile &&) = default;
  Tile &operator=(const Tile &) = default;
  ~Tile();

private:
};
