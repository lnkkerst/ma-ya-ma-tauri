#include "include/TileShieldTree.hpp"
#include "src/lib.rs.h"

#include <iostream>
#include <unordered_set>

TileShieldTree::TileShieldTree() = default;

TileShieldTree::~TileShieldTree() = default;

std::string TileShieldTree::generate_key(int x, int y) {
  return std::to_string(x) + "-" + std::to_string(y);
}

// TODO: return both exposed and unexposed changes
void TileShieldTree::insert(int row, int column, int index,
                            const std::string &tile_id) {
  int sx[] = {row, row, row + 1, row + 1};
  int sy[] = {column, column + 1, column, column + 1};
  for (int i = 0; i < 4; ++i) {
    int x = sx[i];
    int y = sy[i];
    auto key = this->generate_key(x, y);
    auto &tree = this->trees[key];
    auto node = std::make_pair(index, tile_id);
    tree.insert(node);
  }
}

void TileShieldTree::insert(const Tile &tile) {
  this->insert(tile.row, tile.column, tile.index, std::string(tile.id));
}

std::vector<std::string> TileShieldTree::erase(int row, int column, int index,
                                               const std::string &tile_id) {
  std::vector<std::string> res;
  int sx[] = {row, row, row + 1, row + 1};
  int sy[] = {column, column + 1, column, column + 1};
  for (int i = 0; i < 4; ++i) {
    int x = sx[i];
    int y = sy[i];
    auto key = this->generate_key(x, y);
    auto &tree = this->trees[key];
    auto it = tree.find(std::make_pair(index, tile_id));
    if (it != tree.end()) {
      --this->exposed_cnt[tree.rbegin()->second];
      tree.erase(it);
      if (!tree.empty()) {
        ++this->exposed_cnt[tree.rbegin()->second];
        if (this->exposed_cnt[tree.rbegin()->second] == 4) {
          res.emplace_back(tree.rbegin()->second);
        }
      }
    }
  }
  return res;
}

std::vector<std::string> TileShieldTree::erase(const Tile &tile) {
  return this->erase(tile.row, tile.column, tile.index, std::string(tile.id));
}

bool TileShieldTree::check_exposed(int row, int column, int index,
                                   const std::string &tile_id) const {
  auto it = this->exposed_cnt.find(tile_id);
  if (it == this->exposed_cnt.end()) {
    return false;
  }
  return it->second == 4;

  int sx[] = {row, row, row + 1, row + 1};
  int sy[] = {column, column + 1, column, column + 1};
  for (int i = 0; i < 4; ++i) {
    int x = sx[i];
    int y = sy[i];
    auto key = this->generate_key(x, y);
    auto it = this->trees.find(key);
    if (it == this->trees.end()) {
      return false;
    }
    if (it->second.empty()) {
      return false;
    }
    if (it->second.rbegin()->first != index ||
        it->second.rbegin()->second != tile_id) {
      return false;
    }
  }
  return true;
}

bool TileShieldTree::check_exposed(const Tile &tile) const {
  return this->check_exposed(tile.row, tile.column, tile.index,
                             std::string(tile.id));
}

void TileShieldTree::clear() {
  this->trees.clear();
  this->exposed_cnt.clear();
}

void TileShieldTree::init() {
  this->exposed_cnt.clear();
  for (const auto &i : this->trees) {
    if (i.second.empty()) {
      continue;
    }
    ++this->exposed_cnt[i.second.rbegin()->second];
  }
}
