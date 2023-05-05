#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>

#include "include/Game.hpp"
#include "rust/cxx.h"
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
  this->buf_tiles.clear();
  this->tile_diffs.clear();
  this->dropped_tiles.clear();
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

std::unique_ptr<ClickTileResult> Game::handle_click_tile(const Tile &tile) {
  ClickTileResult res;
  if (tile.on_buffer) {
    this->handle_click_buf_tile(tile);
  } else {
    this->handle_click_board_tile(tile);
  }
  auto diffs = this->tile_diffs.send();
  for (const auto &i : diffs) {
    res.diffs.emplace_back(i);
  }
  return std::make_unique<ClickTileResult>(res);
}

// TODO:
void Game::handle_click_buf_tile(const Tile &tile) {}

// TODO:
void Game::handle_click_board_tile(const Tile &tile) {
  if (tile.exposed) {
    append_to_buf(tile);
  }
}

void Game::append_to_buf(const std::string &tile_id) {
  auto tile = this->tiles_map[tile_id];
  if (tile == nullptr) {
    return;
  }

  [&]() {
    for (auto it = this->buf_tiles.begin(); it != this->buf_tiles.end(); ++it) {
      if ((*it)->keyword.content == tile->keyword.content) {
        tile->column = (*it)->column;
        this->tile_diffs.push_back(std::string(tile->id), "column",
                                   tile->column);

        for (auto itr = it; it != this->buf_tiles.end(); ++itr) {
          (*itr)->column += 2;
          this->tile_diffs.push_back(std::string((*itr)->id), "column",
                                     (*itr)->column);
        }

        this->buf_tiles.insert(it, tile);
        return;
      }
    }

    tile->column = this->buf_tiles.size() * 2 + 1;
    this->tile_diffs.push_back(std::string(tile->id), "column", tile->column);
    this->buf_tiles.emplace_back(tile);
  }();

  tile->on_buffer = true;
  this->tile_diffs.push_back(std::string(tile->id), "onBuffer", true);
}

void Game::append_to_buf(const Tile &tile) {
  this->append_to_buf(std::string(tile.id));
}
