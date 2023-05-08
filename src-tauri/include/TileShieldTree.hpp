#pragma once
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

struct Tile;
struct TileDiff;

/**
 * @class TileShieldTree
 * @brief 维护卡牌遮罩关系
 *
 */
class TileShieldTree {
public:
  TileShieldTree();
  TileShieldTree(TileShieldTree &&) = default;
  TileShieldTree(const TileShieldTree &) = default;
  TileShieldTree &operator=(TileShieldTree &&) = default;
  TileShieldTree &operator=(const TileShieldTree &) = default;
  ~TileShieldTree();

  /**
   * @brief 插入卡牌
   *
   * @param row 卡牌行位置
   * @param column 卡牌列位置
   * @param index 卡牌层位置
   * @param tile_id 卡牌 id
   */
  void insert(int row, int column, int index, const std::string &tile_id);

  /**
   * @brief 插入卡牌
   *
   * @param tile 卡牌
   */
  void insert(const Tile &tile);

  /**
   * @brief 插入卡牌
   *
   * @param row 卡牌行位置
   * @param column 卡牌列位置
   * @param index 卡牌层位置
   * @param tile_id 卡牌 id
   * @return 插入卡牌后新暴露的卡牌
   */
  std::vector<std::string> erase(int row, int column, int index,
                                 const std::string &tile_id);

  /**
   * @brief 移走卡牌
   *
   * @param tile 卡牌
   * @return 操作后新暴露出的卡牌
   */
  std::vector<std::string> erase(const Tile &tile);

  /**
   * @brief 检测卡牌是否完全暴露
   *
   * @param row 卡牌行位置
   * @param column 卡牌列位置
   * @param index 卡牌层位置
   * @param tile_id 卡牌 id
   * @return 卡牌是否暴露
   */
  bool check_exposed(int row, int column, int index,
                     const std::string &tile_id) const;

  /**
   * @brief 判断卡牌暴露状态
   *
   * @param tile 卡牌
   * @return 卡牌暴露状态
   */
  bool check_exposed(const Tile &tile) const;

  /**
   * @brief 初始化
   */
  void init();

  /**
   * @brief 清空
   */
  void clear();

private:
  /**
   * @brief 维护每个坐标方格层级最大值
   */
  std::unordered_map<std::string, std::set<std::pair<int, std::string>>> trees;

  /**
   * @brief 维护每个卡牌 1/4 暴露的次数
   */
  std::unordered_map<std::string, int> exposed_cnt;

  /**
   * @brief 根据行和列生成 key
   *
   * @param x 行
   * @param y 列
   */
  static std::string generate_key(int x, int y);
};
