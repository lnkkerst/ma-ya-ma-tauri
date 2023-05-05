#include "include/TileDiffList.hpp"
#include "src/lib.rs.h"
#include <string>

TileDiffList::TileDiffList() = default;

TileDiffList::~TileDiffList() = default;

void TileDiffList::push_back(const TileDiff &diff) {
  this->diffs.emplace_back(diff);
}

void TileDiffList::push_back(const std::string &id, const std::string &field,
                             int value) {
  TileDiff diff;
  diff.id = id;
  diff.diff.emplace_back(field + ":" + std::to_string(value));
  this->push_back(diff);
}

void TileDiffList::push_back(const std::string &id, const std::string &field,
                             std::string &value) {
  TileDiff diff;
  diff.id = id;
  diff.diff.emplace_back(field + ":\"" + value + "\"");
  this->push_back(diff);
}

void TileDiffList::push_back(const std::string &id, const std::string &field,
                             bool value) {
  TileDiff diff;
  diff.id = id;
  diff.diff.emplace_back(field + ":" + (value ? "true" : "false"));
  this->push_back(diff);
}

std::vector<TileDiff> TileDiffList::send() {
  auto res = std::move(this->diffs);
  this->diffs = std::vector<TileDiff>();
  return res;
}

void TileDiffList::clear() { this->diffs.clear(); }
