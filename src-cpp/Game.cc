#include <cstdlib>
#include <ctime>

#include "include/Game.hpp"
#include "src/lib.rs.h"

void Game::load_tiles(const PreLevel &level) {
  this->pre_tiles.clear();
  this->pre_tiles.reserve(level.tiles.size());
  for (const auto &pre_tile : level.tiles) {
    this->pre_tiles.emplace_back(pre_tile);
  }
}

void Game::load_theme(const Theme &theme) {
  this->theme = std::make_shared<Theme>(theme);
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
    auto tile_ptr = std::make_shared<Tile>(tile);
    this->tiles.emplace_back(tile_ptr);
    this->tiles_map[id] = tile_ptr;
    if (tile_ptr->on_buffer) {
      this->buf_tiles.emplace_back(tile_ptr);
    } else {
      this->board_tiles.emplace_back(tile_ptr);
    }
  }
  this->status = GameStatus::Running;
}
