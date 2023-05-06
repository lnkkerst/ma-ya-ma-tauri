#pragma once
#include <string>
#include <vector>

struct TileDiff;

class TileDiffList {
public:
  TileDiffList();
  TileDiffList(TileDiffList &&) = default;
  TileDiffList(const TileDiffList &) = default;
  TileDiffList &operator=(TileDiffList &&) = default;
  TileDiffList &operator=(const TileDiffList &) = default;
  ~TileDiffList();

  void push_back(const TileDiff &diff);
  void push_back(const std::string &id, const std::string &field, int value,
                 int delay = 0);
  void push_back(const std::string &id, const std::string &field,
                 std::string &value, int delay = 0);
  void push_back(const std::string &id, const std::string &field, bool value,
                 int delay = 0);
  void clear();

  std::vector<TileDiff> send();

private:
  std::vector<TileDiff> diffs;
};
