#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

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
  this->tile_diffs.clear();
  this->shield_tree.clear();
  this->score = 0;
  this->buf_tiles.clear();
  this->board_tiles.clear();
  this->dropped_tiles.clear();
  this->board_tiles_set.clear();
  this->buf_tiles_set.clear();
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
      this->board_tiles_set.insert(std::string(tile_ptr->id));
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

  if (this->status != GameStatus::Running) {
    return std::make_unique<ClickTileResult>(res);
  }

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

  if (this->buf_tiles.size() >= 7) {
    this->status = GameStatus::Losed;
  }

  if (this->board_tiles_set.empty()) {
    [&]() {
      for (const auto &i : this->buf_tiles) {
        if (i->keyword.type != "wildcard") {
          this->status = GameStatus::Losed;
          return;
        }
      }
      this->status = GameStatus::Winned;
    }();
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

  this->board_tiles_set.erase(std::string(tile->id));
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
  for (const auto &i : this->buf_tiles) {
    auto key = std::string(i->keyword.content);
    auto &cnt = cnt_single[key];
    ++cnt;
    if (cnt == 3) {
      for (auto iit = this->buf_tiles.begin(); iit != this->buf_tiles.end();
           ++iit) {
        if ((*iit)->keyword.content != key) {
          continue;
        }
        (*iit)->dropped = true;
        this->tile_diffs.push_back(std::string((*iit)->id), "dropped", true,
                                   410);
      }
      auto itb = std::remove_if(this->buf_tiles.begin(), this->buf_tiles.end(),
                                [&](const std::shared_ptr<Tile> &v) -> bool {
                                  return v->keyword.content == key;
                                });
      this->buf_tiles.erase(itb, this->buf_tiles.end());
      this->arrange_buf_tiles(450);

      this->score += 3;
      return;
    }
  }

  std::unordered_map<std::string, int> bk;
  std::vector<std::unordered_set<std::string>> cnt_groups(
      this->theme->groups.size());
  for (int i = 0; i < this->theme->groups.size(); ++i) {
    auto &group = this->theme->groups[i].contents;
    for (const auto &j : group) {
      bk[std::string(j)] = i;
    }
  }

  for (const auto &tile : this->buf_tiles) {
    auto key = std::string(tile->keyword.content);
    if (bk.find(key) == bk.end()) {
      return;
    }
    auto &cnt_group = cnt_groups[bk[key]];
    cnt_group.insert(key);
    if (cnt_group.size() == 2) {
      int cnt_del = 0;
      for (auto it = this->buf_tiles.begin();
           it != this->buf_tiles.end() && cnt_del < 2; ++it) {
        if (cnt_group.find(std::string((*it)->keyword.content)) !=
            cnt_group.end()) {
          (*it)->dropped = true;
          this->tile_diffs.push_back(std::string((*it)->id), "dropped", true,
                                     410);
          cnt_group.erase(std::string((*it)->keyword.content));
          ++cnt_del;
        }
      }
      this->buf_tiles.erase(
          std::remove_if(this->buf_tiles.begin(), this->buf_tiles.end(),
                         [&](const std::shared_ptr<Tile> &v) -> bool {
                           return v->dropped;
                         }),
          this->buf_tiles.end());
      this->arrange_buf_tiles(450);

      this->score += 2;
      return;
    }
  }
}

void Game::arrange_buf_tiles(int move_delay) {
  for (int i = 0; i < this->buf_tiles.size(); ++i) {
    int newVal = i * 2 + 1;
    if (this->buf_tiles[i]->column != newVal) {
      this->buf_tiles[i]->column = newVal;
      this->tile_diffs.push_back(std::string(this->buf_tiles[i]->id), "column",
                                 newVal, move_delay);
    }
  }
}
