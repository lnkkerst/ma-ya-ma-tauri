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
struct TileDiff;

/**
 * @brief 获取当前游戏状态
 *
 * @return 当前游戏状态
 */
GameStatus get_status();

/**
 * @brief 加载牌堆布局
 *
 * @param level 牌堆布局
 */
void load_tiles(const PreLevel &level);

/**
 * @brief 加载主题到当前游戏
 *
 * @param theme 主题
 */
void load_theme(const Theme &theme);

/**
 * @brief 初始化游戏
 */
void init_game();

/**
 * @brief 获取牌堆
 *
 * @return 牌堆
 */
Tiles get_tiles();

/**
 * @brief 处理点击事件
 *
 * @param tile 点击到的卡牌
 *
 * @return 卡牌状态变化
 */
std::unique_ptr<ClickTileResult> handle_click_tile(const Tile &tile);

/**
 * @brief 获取当前分数
 *
 * @return 分数
 */
int get_score();
