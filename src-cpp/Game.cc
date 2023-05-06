#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>

#include "include/Game.hpp"
#include "include/utils.hpp"
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

GameStatus Game::get_status() const noexcept { return this->status; }

rust::Vec<Tile> Game::get_tiles() const {
  rust::Vec<Tile> res;
  for (const auto &tile : this->tiles) {
    res.emplace_back(*tile);
  }
  return res;
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
  this->shield_tree.clear();

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
    this->shield_tree.insert(*tile_ptr);
    tile_ptr->exposed = this->shield_tree.check_exposed(*tile_ptr);
  }

  this->status = GameStatus::Running;
  this->shield_tree.init();
  this->build_shield();
  this->tile_diffs.send();
}

std::unique_ptr<ClickTileResult> Game::handle_click_tile(const Tile &ori_tile) {
  ClickTileResult res;
  auto it = this->tiles_map.find(std::string(ori_tile.id));
  if (it == this->tiles_map.end()) {
    return std::make_unique<ClickTileResult>(res);
  }

  auto tile = it->second;

  if (tile->on_buffer) {
    this->handle_click_buf_tile(*tile);
  } else {
    this->handle_click_board_tile(*tile);
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
  auto it = this->tiles_map.find(tile_id);
  if (it == this->tiles_map.end()) {
    return;
  }
  auto tile = it->second;

  auto to_exposed = this->shield_tree.erase(*tile);
  for (const auto &i : to_exposed) {
    this->tile_diffs.push_back(i, "exposed", true);
    this->tiles_map[i]->exposed = true;
  }

  [&]() {
    for (auto it = this->buf_tiles.begin(); it != this->buf_tiles.end(); ++it) {
      if ((*it)->keyword.content == tile->keyword.content) {
        while (it != this->buf_tiles.end() &&
               (*it)->keyword.content == tile->keyword.content) {
          ++it;
        }
        if (it == this->buf_tiles.end()) {
          break;
        }

        tile->column = (*it)->column;
        this->tile_diffs.push_back(std::string(tile->id), "column",
                                   tile->column);

        for (auto itr = it; itr != this->buf_tiles.end(); ++itr) {
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
  this->drop_tiles();
}

void Game::append_to_buf(const Tile &tile) {
  this->append_to_buf(std::string(tile.id));
}

int Game::get_score() const { return this->score; }

void Game::build_shield() {
  for (const auto &i : this->tiles) {
    bool newVal = this->shield_tree.check_exposed(*i);
    if (i->exposed != newVal) {
      i->exposed = newVal;
      this->tile_diffs.push_back(std::string(i->id), "exposed", newVal);
    }
  }
}

void Game::drop_tiles() {
  std::unordered_map<std::string, int> cnt_single;
  std::unordered_map<std::string, int> cnt_group;
}
