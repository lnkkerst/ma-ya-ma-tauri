#pragma once
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "include/TileDiffList.hpp"
#include "include/TileShieldTree.hpp"
#include "rust/cxx.h"

enum class GameStatus : std::uint8_t;
struct PreTile;
struct PreLevel;
struct Tile;
struct Tiles;
struct Theme;
struct ClickTileResult;
struct TileDiff;

class Game {
public:
  Game() = default;
  Game(Game &&) = default;
  Game(const Game &) = default;
  Game &operator=(Game &&) = default;
  Game &operator=(const Game &) = default;
  ~Game() = default;

  GameStatus get_status() const noexcept;
  void load_tiles(const PreLevel &level);
  void load_theme(const Theme &theme);
  void init_game();
  rust::Vec<Tile> get_tiles() const;
  std::unique_ptr<ClickTileResult> handle_click_tile(const Tile &tile);
  int get_score() const;

  GameStatus status;

private:
  int score;
  std::unordered_map<std::string, std::shared_ptr<Tile>> tiles_map;
  std::vector<std::shared_ptr<Tile>> tiles;
  std::vector<PreTile> pre_tiles;
  std::shared_ptr<Theme> theme;

  std::vector<std::shared_ptr<Tile>> board_tiles;
  std::vector<std::shared_ptr<Tile>> buf_tiles;
  std::vector<std::shared_ptr<Tile>> dropped_tiles;
  std::unordered_set<std::string> board_tiles_set;
  std::unordered_set<std::string> buf_tiles_set;
  std::unordered_set<std::string> dropped_tiles_set;
  std::unordered_set<std::string> selected_tiles_set;

  TileDiffList tile_diffs;

  TileShieldTree shield_tree;

  void handle_click_buf_tile(const Tile &tile);
  void handle_click_board_tile(const Tile &tile);
  void append_to_buf(const Tile &tile);
  void append_to_buf(const std::string &tile_id);
  void build_shield();
  void drop_paired_tiles();
  void drop_all_buf_tiles();
  void drop_tile(std::shared_ptr<Tile> tile);
  void arrange_buf_tiles(int move_delay = 0);
  void clear_selected();
};
