#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>

#include "include/mayama.h"
#include "src/lib.rs.h"

std::shared_ptr<Game> game;

void Game::load_tiles(const PreLevel &level) {
  this->pre_tiles.clear();
  this->pre_tiles.reserve(level.tiles.size());
  for (const auto &pre_tile : level.tiles) {
    this->pre_tiles.emplace_back(pre_tile);
  }
}

void Game::load_theme(const Theme &theme) {
  this->theme = std::shared_ptr<Theme>(new Theme(theme));
}

// TODO: return GAME STATUS
GameStatus Game::get_status() const noexcept { return GameStatus::Running; }

rust::Vec<Tile> Game::get_tiles() const {
  rust::Vec<Tile> res;
  for (const auto &tile : this->tiles) {
    res.emplace_back(*tile);
  }
  return res;
}

std::string gen_random(const int len = 64) {
  static const char alphanum[] = "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz";
  std::string tmp_s;
  tmp_s.reserve(len);

  for (int i = 0; i < len; ++i) {
    tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  return tmp_s;
}

void Game::init_game() {
  if (this->theme == nullptr) {
    throw "theme not load";
  }
  std::srand(std::time(nullptr));
  this->tiles.clear();
  this->tiles_map.clear();
  auto keywords = this->theme->keywords;
  for (const auto &pre_tile : this->pre_tiles) {
    Tile tile;
    tile.column = pre_tile.column;
    tile.row = pre_tile.row;
    tile.index = pre_tile.index;
    tile.dropped = false;
    tile.exposed = false;
    tile.keyword = keywords[std::rand() % keywords.size()];
    tile.on_buffer = false;
    tile.selected = false;
    auto id = gen_random();
    tile.id = id;
    this->tiles.emplace_back(std::make_shared<Tile>(tile));
    this->tiles_map[id] = this->tiles.back();
  }
  this->status = GameStatus::Running;
}

void load_tiles(const PreLevel &level) { game->load_tiles(level); }

void load_theme(const Theme &theme) { game->load_theme(theme); }

GameStatus get_status() { return game->get_status(); }

Tiles get_tiles() {
  Tiles res;
  res.value = game->get_tiles();
  return res;
}
