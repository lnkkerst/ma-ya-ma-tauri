#pragma once
#include <string>
#include <vector>

struct TileDiff;

// 维护卡牌状态变化
class TileDiffList {
public:
  TileDiffList();
  TileDiffList(TileDiffList &&) = default;
  TileDiffList(const TileDiffList &) = default;
  TileDiffList &operator=(TileDiffList &&) = default;
  TileDiffList &operator=(const TileDiffList &) = default;
  ~TileDiffList();

  /**
   * @brief 插入一组新变化
   *
   * @param diff 变化
   */
  void push_back(const TileDiff &diff);

  /**
   * @brief 插入一组新变化
   *
   * @param id 卡牌 id
   * @param field 改变的字段
   * @param value 改变后的值
   * @param delay 改变的延迟
   */
  void push_back(const std::string &id, const std::string &field, int value,
                 int delay = 0);

  /**
   * @brief 插入一组新变化
   *
   * @param id 卡牌 id
   * @param field 改变的字段
   * @param value 改变后值改变的字
   * @param delay 改变的延迟
   */
  void push_back(const std::string &id, const std::string &field,
                 std::string &value, int delay = 0);

  /**
   * @brief 插入一组新变化
   *
   * @param id 卡牌 id
   * @param field 改变的字段
   * @param value 改变后的值
   * @param delay 改变的延迟
   */
  void push_back(const std::string &id, const std::string &field, bool value,
                 int delay = 0);

  /**
   * @brief 清空列表
   */
  void clear();

  /**
   * @brief 消费列表
   * @return 变化
   */
  std::vector<TileDiff> send();

private:
  /**
   * @brief 列表
   */
  std::vector<TileDiff> diffs;
};
