#pragma once
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "include/TileDiffList.hpp"
#include "include/TileShieldTree.hpp"
#include "rust/cxx.h"

enum class GameStatus : std::uint8_t;
struct PreTile;
struct PreLevel;
struct Tile;
struct Tiles;
struct Theme;
struct ClickTileResult;
struct TileDiff;

/**
 * @class Game
 * @brief 游戏
 *
 */
class Game {
public:
  Game() = default;
  Game(Game &&) = default;
  Game(const Game &) = default;
  Game &operator=(Game &&) = default;
  Game &operator=(const Game &) = default;
  ~Game() = default;

  /**
   * @brief 获取游戏胜负状态
   *
   * @return 游戏胜负状态
   */
  GameStatus get_status() const noexcept;

  /**
   * @brief 加载卡牌布局
   *
   * @param level 卡牌布局
   */
  void load_tiles(const PreLevel &level);

  /**
   * @brief 加载主题
   *
   * @param theme 主题
   */
  void load_theme(const Theme &theme);

  /**
   * @brief 初始化游戏
   */
  void init_game();

  /**
   * @brief 获取初始后的牌堆
   *
   * @return 牌堆
   */
  rust::Vec<Tile> get_tiles() const;

  /**
   * @brief 处理点击
   *
   * @param tile 点击的卡牌
   *
   * @return 卡牌状态变化
   */
  std::unique_ptr<ClickTileResult> handle_click_tile(const Tile &tile);

  /**
   * @brief 获取分数
   *
   * @return 分数
   */
  int get_score() const;

  /**
   * @brief 游戏胜负状态
   */
  GameStatus status;

private:
  /**
   * @brief 游戏分数
   */
  int score;

  /**
   * @brief 通过 id 标记卡牌
   */
  std::unordered_map<std::string, std::shared_ptr<Tile>> tiles_map;

  /**
   * @brief 牌堆
   */
  std::vector<std::shared_ptr<Tile>> tiles;

  /**
   * @brief 牌堆布局
   */
  std::vector<PreTile> pre_tiles;

  /**
   * @brief 主题
   */
  std::shared_ptr<Theme> theme;

  std::vector<std::shared_ptr<Tile>> board_tiles;
  std::vector<std::shared_ptr<Tile>> buf_tiles;
  std::vector<std::shared_ptr<Tile>> dropped_tiles;
  std::unordered_set<std::string> board_tiles_set;
  std::unordered_set<std::string> buf_tiles_set;
  std::unordered_set<std::string> dropped_tiles_set;
  std::unordered_set<std::string> selected_tiles_set;

  /**
   * @brief 记录卡牌状态变化
   */
  TileDiffList tile_diffs;

  /**
   * @brief 维护遮罩关系
   */
  TileShieldTree shield_tree;

  /**
   * @brief 处理点击缓冲区的卡牌
   *
   * @param tile 点击的卡牌
   */
  void handle_click_buf_tile(const Tile &tile);

  /**
   * @brief 处理点击牌堆的卡牌
   *
   * @param tile 卡牌
   */
  void handle_click_board_tile(const Tile &tile);

  /**
   * @brief 把牌堆的卡牌移动到缓冲区
   *
   * @param tile 牌堆的卡牌
   */
  void append_to_buf(const Tile &tile);
  /**
   * @brief 把牌堆的卡牌移动到缓冲区
   *
   * @param tile_id 卡牌 id
   */
  void append_to_buf(const std::string &tile_id);

  /**
   * @brief 初始化牌堆遮罩关系
   */
  void build_shield();

  /**
   * @brief 消除缓冲区配对的卡牌
   */
  void drop_paired_tiles();

  /**
   * @brief 清空缓冲区
   */
  void drop_all_buf_tiles();

  /**
   * @brief 消除单张卡牌
   *
   * @param tile 待消除的卡牌
   */
  void drop_tile(std::shared_ptr<Tile> tile);

  /**
   * @brief 重新排布缓冲区卡牌
   *
   * @param move_delay 移动延迟
   */
  void arrange_buf_tiles(int move_delay = 0);

  /**
   * @brief 清除缓冲区卡牌选中状态
   */
  void clear_selected();
};
