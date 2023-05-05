#pragma once
#include <memory>
#include <unordered_map>

#include "rust/cxx.h"

enum class GameStatus : std::uint8_t;
struct PreTile;
struct PreLevel;
struct Tile;
struct Tiles;
struct Theme;
struct ClickTileResult;

class Game {
public:
  Game() = default;
  Game(Game &&) = default;
  Game(const Game &) = default;
  Game &operator=(Game &&) = default;
  Game &operator=(const Game &) = default;
  ~Game() = default;

  std::unordered_map<std::string, std::shared_ptr<Tile>> tiles_map;
  std::vector<std::shared_ptr<Tile>> tiles;
  std::vector<PreTile> pre_tiles;
  std::shared_ptr<Theme> theme;

  GameStatus get_status() const noexcept;
  void load_tiles(const PreLevel &level);
  void load_theme(const Theme &theme);
  void init_game();
  rust::Vec<Tile> get_tiles() const;
  std::unique_ptr<ClickTileResult> handle_click_tile(const Tile &tile);

  GameStatus status;

private:
  std::vector<std::shared_ptr<Tile>> board_tiles;
  std::vector<std::shared_ptr<Tile>> buf_tiles;

  void handle_click_buf_tile(const Tile &tile, rust::Vec<rust::String> &diffs);
  void handle_click_board_tile(const Tile &tile,
                               rust::Vec<rust::String> &diffs);
};
