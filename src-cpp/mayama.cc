#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>

#include "include/Game.hpp"
#include "include/mayama.hpp"
#include "src/lib.rs.h"

Game game;

void load_tiles(const PreLevel &level) { game.load_tiles(level); }

void load_theme(const Theme &theme) { game.load_theme(theme); }
GameStatus get_status() { return game.get_status(); }

Tiles get_tiles() {
  Tiles res;
  res.value = game.get_tiles();
  return res;
}

void init_game() { game.init_game(); }

std::unique_ptr<ClickTileResult> Game::handle_click_tile(const Tile &tile) {
  ClickTileResult res;
}

std::unique_ptr<ClickTileResult> handle_click_tile(const Tile &tile) {
  return game.handle_click_tile(tile);
}
