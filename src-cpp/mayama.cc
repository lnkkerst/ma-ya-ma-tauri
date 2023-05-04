#include "include/mayama.h"
#include "src/lib.rs.h"

Game::Game() {}

Game::~Game() {}

std::unique_ptr<Game> new_game() { return std::unique_ptr<Game>(new Game()); }

void test() {
  Keyword a;
  Tile b;
}
