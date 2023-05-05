#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "rust/cxx.h"

enum class GameStatus : std::uint8_t;
struct PreTile;
struct PreLevel;
struct Tile;
struct Tiles;
struct Theme;

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
  GameStatus status;

private:
};

GameStatus get_status();

void load_tiles(const PreLevel &level);

void load_theme(const Theme &theme);

void init_game();

Tiles get_tiles();

std::string gen_random(const int len);
