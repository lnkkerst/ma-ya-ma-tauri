#pragma once
#include <memory>
#include <unordered_map>

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

  TileDiffList tile_diffs;

  TileShieldTree shield_tree;

  void handle_click_buf_tile(const Tile &tile);
  void handle_click_board_tile(const Tile &tile);
  void append_to_buf(const Tile &tile);
  void append_to_buf(const std::string &tile_id);
  void build_shield();
  void drop_tiles();
};
