#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

enum class GameStatus : std::uint8_t;
struct PreTile;
struct PreLevel;
struct Tile;
struct Tiles;
struct Theme;
struct ClickTileResult;

GameStatus get_status();

void load_tiles(const PreLevel &level);

void load_theme(const Theme &theme);

void init_game();

Tiles get_tiles();

std::string gen_random(const int len);

std::unique_ptr<ClickTileResult> handle_click_tile(const Tile &tile);
