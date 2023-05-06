#include "include/TileDiffList.hpp"
#include "src/lib.rs.h"
#include <string>

TileDiffList::TileDiffList() = default;

TileDiffList::~TileDiffList() = default;

void TileDiffList::push_back(const TileDiff &diff) {
  this->diffs.emplace_back(diff);
}

void TileDiffList::push_back(const std::string &id, const std::string &field,
                             int value, int delay) {
  TileDiff diff;
  diff.id = id;
  auto s = field + ":" + std::to_string(value);
  if (delay) {
    s += ":" + std::to_string(delay);
  }
  diff.diff.emplace_back(s);
  this->push_back(diff);
}

void TileDiffList::push_back(const std::string &id, const std::string &field,
                             std::string &value, int delay) {
  TileDiff diff;
  diff.id = id;
  auto s = field + ":\"" + value + "\"";
  if (delay) {
    s += ":" + std::to_string(delay);
  }
  diff.diff.emplace_back(s);
  this->push_back(diff);
}

void TileDiffList::push_back(const std::string &id, const std::string &field,
                             bool value, int delay) {
  TileDiff diff;
  diff.id = id;
  auto s = field + ":" + (value ? "true" : "false");
  if (delay) {
    s += ":" + std::to_string(delay);
  }
  diff.diff.emplace_back(s);
  this->push_back(diff);
}

std::vector<TileDiff> TileDiffList::send() {
  auto res = std::move(this->diffs);
  this->diffs = std::vector<TileDiff>();
  return res;
}

void TileDiffList::clear() { this->diffs.clear(); }
