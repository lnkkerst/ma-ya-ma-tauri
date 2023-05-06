#pragma once
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

struct Tile;
struct TileDiff;

class TileShieldTree {
public:
  TileShieldTree();
  TileShieldTree(TileShieldTree &&) = default;
  TileShieldTree(const TileShieldTree &) = default;
  TileShieldTree &operator=(TileShieldTree &&) = default;
  TileShieldTree &operator=(const TileShieldTree &) = default;
  ~TileShieldTree();

  void insert(int row, int column, int index, const std::string &tile_id);
  void insert(const Tile &tile);
  std::vector<std::string> erase(int row, int column, int index,
                                 const std::string &tile_id);
  std::vector<std::string> erase(const Tile &tile);
  bool check_exposed(int row, int column, int index,
                     const std::string &tile_id) const;
  bool check_exposed(const Tile &tile) const;
  void init();

  void clear();

private:
  std::unordered_map<std::string, std::set<std::pair<int, std::string>>> trees;
  std::unordered_map<std::string, int> exposed_cnt;

  static std::string generate_key(int x, int y);
};
